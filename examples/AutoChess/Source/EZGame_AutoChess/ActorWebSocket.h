// by Tiax 2020/11/26

#pragma once

#include "CoreMinimal.h"
#include <future>
#include <memory>
#include "GameFramework/Actor.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "ActorWebSocket.generated.h"

UCLASS()
class EZGAME_AUTOCHESS_API AActorWebSocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorWebSocket();

	const FString ServerURL="ws://game.ohtoai.top/websocket";
	const FString ServerProtocol="ws";

	TSharedPtr<IWebSocket> Socket=nullptr;

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode,const FString& Reason,bool bWasClean);
	void OnMessage(const FString& Message); // 接收消息时
	void OnMessageSent(const FString& MessageString); // 发送消息时

public:	
	std::shared_ptr<std::promise<FString>> messageReceivedPromise;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "WebSocketService")
	void SendMessage(const FString message);

	UFUNCTION(BlueprintCallable, Category = "WebSocketService")
	void SendMessageSync(const FString message, FString& ReceivedMessage);

	void Heartbeat();
};
