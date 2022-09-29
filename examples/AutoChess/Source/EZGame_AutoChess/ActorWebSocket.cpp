// by Tiax 2020/11/26

#include "ActorWebSocket.h"

// Sets default values
AActorWebSocket::AActorWebSocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AActorWebSocket::BeginPlay()
{
	Super::BeginPlay();
	
	FModuleManager::Get().LoadModuleChecked("WebSockets");

	Socket=FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);

	Socket->OnConnected().AddUObject(this,&AActorWebSocket::OnConnected);
	Socket->OnConnectionError().AddUObject(this,&AActorWebSocket::OnConnectionError);
	Socket->OnClosed().AddUObject(this,&AActorWebSocket::OnClosed);
	Socket->OnMessage().AddUObject(this,&AActorWebSocket::OnMessage);
	Socket->OnMessageSent().AddUObject(this,&AActorWebSocket::OnMessageSent);

	Socket->Connect();

	UE_LOG(LogTemp, Warning, TEXT("AActorWebSocket Init"));
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AActorWebSocket::Heartbeat, 5, true, 1);
}

void AActorWebSocket::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Socket->Close();
}

// Called every frame
void AActorWebSocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActorWebSocket::OnConnected()
{
	UE_LOG(LogTemp,Warning,TEXT("%s"),*FString(__FUNCTION__));
}

void AActorWebSocket::OnConnectionError(const FString& Error)
{
	messageReceivedPromise->set_exception(std::make_exception_ptr(std::runtime_error(TCHAR_TO_UTF8(*Error))));
	UE_LOG(LogTemp,Warning,TEXT("%s Error:%s"),*FString(__FUNCTION__),*Error);
}

void AActorWebSocket::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogTemp,Warning,TEXT("%s StatusCode:%d Reason:%s bWasClean:%d"),
		*FString(__FUNCTION__),StatusCode,*Reason,bWasClean);
}

void AActorWebSocket::OnMessage(const FString& Message)
{
	if (messageReceivedPromise)
	{
		messageReceivedPromise->set_value(Message);
		messageReceivedPromise.reset();
	}
	UE_LOG(LogTemp,Warning,TEXT("%s Message:%s"),*FString(__FUNCTION__),*Message);

	UE_LOG(LogTemp,Warning,TEXT("%s"),*Message);
	//JsonParse(Message);
}

void AActorWebSocket::OnMessageSent(const FString& MessageString)
{
	UE_LOG(LogTemp,Warning,TEXT("%s MessageString:%s"),*FString(__FUNCTION__),*MessageString);
}

void AActorWebSocket::Heartbeat()
{
	//TSharedPtr<FJsonObject> RootObj=MakeShareable(new FJsonObject);
	//RootObj->SetBoolField("IsExecute",true);
	//RootObj->SetStringField("Type","Test");
	//TSharedPtr<FJsonValue> Value=MakeShareable(new FJsonValueNumber(GetGameTimeSinceCreation()));
	//RootObj->SetField("Value",Value);

	//FString JsonStr;
	//TSharedRef<TJsonWriter<TCHAR>> JsonWriter=TJsonWriterFactory<TCHAR>::Create(&JsonStr);
	//FJsonSerializer::Serialize(RootObj.ToSharedRef(),JsonWriter);
	//
	//if(Socket->IsConnected())
	//{
	//	// Socket->Send(FString::SanitizeFloat(GetGameTimeSinceCreation()));
	//	Socket->Send(JsonStr);
	//}

	UE_LOG(LogTemp, Warning, TEXT("%s Result: Test ZhuGF"), *FString(__FUNCTION__));
}

void AActorWebSocket::SendMessage(const FString InMessage)
{
	if (Socket->IsConnected())
	{
		Socket->Send(InMessage);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s Result:%s"), *FString(__FUNCTION__), *InMessage);
}

void AActorWebSocket::SendMessageSync(const FString InMessage, FString&ReceivedMessage)
{
	messageReceivedPromise = std::make_shared<std::promise<FString>>();
	auto messageReceivedFuture = messageReceivedPromise->get_future();
	if (Socket->IsConnected())
	{
		Socket->Send(InMessage);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s Result:%s"), *FString(__FUNCTION__), *InMessage);

	ReceivedMessage = messageReceivedFuture.get();
}