// Fill out your copyright notice in the Description page of Project Settings.


#include "OhtoAiWebSocketSession.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"


UOhtoAiWebSocketSession* UOhtoAiWebSocketSession::CreateOhtoAiWebSocketSession(const FString& Url)
{
	UOhtoAiWebSocketSession* ptr =  NewObject<UOhtoAiWebSocketSession>();
	ptr->m_Url = Url;
	return ptr;
}

UOhtoAiWebSocketSession* UOhtoAiWebSocketSession::OhtoAiWebSocketSessionJayzhouInstance()
{
	static UOhtoAiWebSocketSession* ptr = [] {
		auto session = NewObject<UOhtoAiWebSocketSession>();
		session->m_Url = "ws://47.103.124.58:10100/websocket";
		session->AddToRoot();
		return session;
	}();
	return ptr;
}

void UOhtoAiWebSocketSession::Activate()
{
	GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Blue
		, FString::Printf(TEXT("UOhtoAiWebSocketSession Activate url=%s"), *m_Url));
	Connect();
}

void UOhtoAiWebSocketSession::Connect()
{
	if (nullptr != m_NativeSocket)
	{
		GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Red
			, FString::Printf(TEXT("UOhtoAiWebSocketSession nullptr != m_NativeSocket")));
		//return;
	}
	m_NativeSocket = FWebSocketsModule::Get().CreateWebSocket(m_Url);
	m_NativeSocket->OnConnected().AddUObject(this, &UOhtoAiWebSocketSession::OnConnected);
	m_NativeSocket->OnClosed().AddUObject(this, &UOhtoAiWebSocketSession::OnClosed);
	m_NativeSocket->OnMessage().AddUObject(this, &UOhtoAiWebSocketSession::OnMessage);
	m_NativeSocket->OnMessageSent().AddUObject(this, &UOhtoAiWebSocketSession::OnMessageSent);
	m_NativeSocket->OnRawMessage().AddUObject(this, &UOhtoAiWebSocketSession::OnRawMessage);
	m_NativeSocket->OnConnectionError().AddUObject(this, &UOhtoAiWebSocketSession::OnError);

	m_NativeSocket->Connect();
}

void UOhtoAiWebSocketSession::Close()
{
	if (false == IsConnected())
	{
		m_NativeSocket = nullptr;
		SetReadyToDestroy();
		return;
	}
	m_NativeSocket->Close();
	m_NativeSocket = nullptr;
	SetReadyToDestroy();
}

void UOhtoAiWebSocketSession::SendMessage(const FString& Message)
{
	if (false == IsConnected())
	{
		GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Red
			, FString::Printf(TEXT("UOhtoAiWebSocketSession IsConnected false")));
		return;
	}
	if (Message.Len() <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Red
			, FString::Printf(TEXT("UOhtoAiWebSocketSession SendMessage Message is empty")));
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Blue
		, FString::Printf(TEXT("UOhtoAiWebSocketSession SendMessage Message=%s"), *Message));
	m_NativeSocket->Send(Message);
}

void UOhtoAiWebSocketSession::SendRawMessage(TArray<uint8> Message)
{
	if (false == IsConnected())
	{
		GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Red
			, FString::Printf(TEXT("UOhtoAiWebSocketSession IsConnected false")));
		return;
	}
	if (Message.Num() <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Red
			, FString::Printf(TEXT("UOhtoAiWebSocketSession SendRawMessage Data is empty")));
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Blue
		, FString::Printf(TEXT("UOhtoAiWebSocketSession SendRawMessage DataSize = %d"), Message.Num()));
	m_NativeSocket->Send(Message.GetData(), Message.Num(), true);
}

bool UOhtoAiWebSocketSession::IsConnected()
{
	if (nullptr == m_NativeSocket)
	{
		return false;
	}
	return m_NativeSocket->IsConnected();

}

void UOhtoAiWebSocketSession::WebsocketUrl(FString& WebsocketUrl)
{
	WebsocketUrl = m_Url;
}

void UOhtoAiWebSocketSession::OnConnected()
{
	GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Blue
		, FString::Printf(TEXT("UOhtoAiWebSocketSession OnConnected Success url=%s"), *m_Url));

	OnConnectedEvent.Broadcast(this, "", {}, 0);
}

void UOhtoAiWebSocketSession::OnError(const FString& Error)
{
	GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Blue
		, FString::Printf(TEXT("UOhtoAiWebSocketSession OnError url=%s error=%s"), *m_Url, *Error));
	OnClosedEvent.Broadcast(this, Error, {}, 0);
}

void UOhtoAiWebSocketSession::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Blue
		, FString::Printf(TEXT("UOhtoAiWebSocketSession OnClosed  url=%s StatusCode=%d Reason=%s bWasClean=%d"),
			*m_Url, StatusCode, *Reason, bWasClean));

	OnClosedEvent.Broadcast(this, Reason, {}, 0);
}

void UOhtoAiWebSocketSession::OnMessage(const FString& MessageString)
{
	GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Blue
		, FString::Printf(TEXT("UOhtoAiWebSocketSession OnMessage MessageString=%s"), *MessageString));

	OnMessageEvent.Broadcast(this, MessageString, {}, 0);
}

void UOhtoAiWebSocketSession::OnMessageSent(const FString& MessageString)
{
	GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Blue
		, FString::Printf(TEXT("UOhtoAiWebSocketSession OnMessageSent MessageString=%s"), *MessageString));

	OnMessageSentEvent.Broadcast(this, MessageString, {}, 0);
}

void UOhtoAiWebSocketSession::OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
	GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Blue
		, FString::Printf(TEXT("UOhtoAiWebSocketSession OnRawMessage Size=%d BytesRemaining=%d"), Size, BytesRemaining));
	TArray<uint8> Message;
	Message.Append(static_cast<const uint8*>(Data), Size);
	OnRawMessageEvent.Broadcast(this, "", Message, BytesRemaining);
}