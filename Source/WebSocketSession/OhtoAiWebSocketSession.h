// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OhtoAiWebSocketSession.generated.h"

class IWebSocket;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class THEPARTY_API UOhtoAiWebSocketSession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "OhtoAi | WebSocketSession")
		static UOhtoAiWebSocketSession* CreateOhtoAiWebSocketSession(const FString& WebsocketUrl);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Jayzhou Websocket Session Instance"),  Category = "OhtoAi | WebSocketSession | Instance")
		static UOhtoAiWebSocketSession* OhtoAiWebSocketSessionJayzhouInstance();

	UFUNCTION(BlueprintCallable, Category = "OhtoAi | WebSocketSession")
		void Close();

	UFUNCTION(BlueprintCallable, Category = "OhtoAi | WebSocketSession")
		void SendMessage(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "OhtoAi | WebSocketSession")
		void SendRawMessage(TArray<uint8> Message);
	
	UFUNCTION(BlueprintCallable, Category = "OhtoAi | WebSocketSession")
		bool IsConnected();

	UFUNCTION(BlueprintCallable, Category = "OhtoAi | WebSocketSession")
		void WebsocketUrl(FString& WebsocketUrl);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOhtoAiWebSocketRawMessageEvent
		, const UOhtoAiWebSocketSession*, websocket
		, const FString&, MessageString
		, const TArray<uint8>&, MessageData
		, int, BytesRemaining);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintAssignable, Category = "OhtoAi | WebSocketSession")
		FOhtoAiWebSocketRawMessageEvent OnConnectedEvent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintAssignable, Category = "OhtoAi | WebSocketSession")
		FOhtoAiWebSocketRawMessageEvent OnMessageSentEvent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintAssignable, Category = "OhtoAi | WebSocketSession")
		FOhtoAiWebSocketRawMessageEvent OnMessageEvent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintAssignable, Category = "OhtoAi | WebSocketSession")
		FOhtoAiWebSocketRawMessageEvent OnRawMessageEvent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintAssignable, Category = "OhtoAi | WebSocketSession")
		FOhtoAiWebSocketRawMessageEvent OnClosedEvent;


	virtual void Activate();
	void Connect();
private:
	void OnConnected();
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& MessageString );
	void OnMessageSent(const FString& MessageString);
	void OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);
	void OnError(const FString& Error);
public:
	FString m_Url;
private:
	TSharedPtr<IWebSocket> m_NativeSocket = nullptr;
	TMap<FString, FString> m_Headers;
};
