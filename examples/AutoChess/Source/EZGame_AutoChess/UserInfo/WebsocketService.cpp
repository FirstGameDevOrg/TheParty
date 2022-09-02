// Fill out your copyright notice in the Description page of Project Settings.


#include "WebsocketService.h"

#include "Kismet/KismetSystemLibrary.h"


#include "MessageWrapper.hpp"


// Sets default values
AWebsocketService::AWebsocketService()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CommandRouterMap::instance().fromString(
    R"pb.socket({
    "pb.socket.UserLogin": {
        "high": 15,
        "low": 0
    },
    "pb.socket.HelloSpringMsg": {
        "high": 2,
        "low": 0
    })pb.socket");

	using pb::socket::UserInfo;



	// Set logging to be pretty verbose (everything except message payloads)
	client.set_access_channels(websocketpp::log::alevel::all);
	client.clear_access_channels(websocketpp::log::alevel::frame_payload);

	// Initialize ASIO
	client.init_asio();

	// Register our message handler
	client.set_message_handler([&](websocketpp::connection_hdl hdl, websocketpp::config::asio_client::message_type::ptr msg) {
		auto userInfo = MessageWrapper::FromString(msg->get_payload()).object<UserInfo>();

		spdlog::info("user.name = {}, user.id = {}, user.jwt = {}", userInfo.name(), userInfo.id(), userInfo.jwt());
		});


	;
	client.set_open_handler([&](websocketpp::lib::weak_ptr<void> d) {

		MessageWrapper wrapper;

		pb::socket::UserLogin userLogin;
		userLogin.set_username("user");
		userLogin.set_password("123");

		wrapper.setRouter(userLogin);
		wrapper.setData(userLogin);

		client.send(connection->get_handle(), wrapper.serializeAsString(), websocketpp::frame::opcode::binary);
		});
	websocketpp::lib::error_code ec;
	connection = client.get_connection("ws://game.ohtoai.top/websocket", ec);

	if (ec) {
		UKismetSystemLibrary::PrintString(this, ec.message().c_str());
		return;
	}
	// Note that connect here only requests a connection. No network messages are
	// exchanged until the event loop starts running in the next line.
	client.connect(connection);

	std::thread([&] {
		try {
			client.run();
		}
		catch (websocketpp::exception const& e) {
			UKismetSystemLibrary::PrintString(this, e.what());
		}}).join();
}

// Called when the game starts or when spawned
void AWebsocketService::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWebsocketService::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

