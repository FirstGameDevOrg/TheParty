// Fill out your copyright notice in the Description page of Project Settings.


#include "UserLogin.h"
#include "MessageWrapper.hpp"

// Sets default values
AUserLogin::AUserLogin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

bool AUserLogin::Login(FString UserName, FString Password)
{
	UE_LOG(LogTemp, Warning, TEXT("%s with UserName = %s"), *FString(__FUNCTION__), *UserName);
	MessageWrapper wrapper;

	pb::socket::UserLogin userLogin;
	userLogin.set_username(TCHAR_TO_UTF8(*UserName));
	userLogin.set_password(TCHAR_TO_UTF8(*Password));
	wrapper.setRouter(userLogin);
	wrapper.setData(userLogin);

	FString loginMessage;
	client->SendMessageSync(wrapper.serializeAsString().c_str(), loginMessage);
	
	MessageWrapper wrapper_receive;
	wrapper_receive.parseFromString(TCHAR_TO_UTF8(*loginMessage));
	pb::socket::UserInfo user_info;
	wrapper_receive.toObject(user_info);
	UE_LOG(LogTemp, Warning, TEXT("%s ID = %d, Name = %s"), *FString(__FUNCTION__), user_info.id(), user_info.name().c_str());

	return UserName == user_info.name().c_str();
}


// Called when the game starts or when spawned
void AUserLogin::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("%s UserLogin Init ----"), *FString(__FUNCTION__));

	client = NewObject<AActorWebSocket>();
	client->BeginPlay();
	//client->DispatchBeginPlay();
	
	CommandRouterMap::instance().fromString(R"({"pb.socket.UserLogin":{"high":15,"low":0},"pb.socket.HelloSpringMsg":{"high":2,"low":0}})");
	
}


void AUserLogin::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	client->EndPlay(EndPlayReason);
}

// Called every frame
void AUserLogin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

