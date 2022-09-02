// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/common/thread.hpp>
#include "socket.pb.h"
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"


#include "WebsocketService.generated.h"

UCLASS()
class EZGAME_AUTOCHESS_API AWebsocketService : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWebsocketService();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	websocketpp::client<websocketpp::config::asio_client> client;
	websocketpp::client<websocketpp::config::asio_client>::connection_ptr connection;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
