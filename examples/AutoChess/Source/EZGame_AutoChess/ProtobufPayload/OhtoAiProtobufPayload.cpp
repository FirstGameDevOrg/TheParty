// Fill out your copyright notice in the Description page of Project Settings.

#include "OhtoAiProtobufPayload.h"
#include "MessageWrapper.hpp"

bool UOhtoAiProtobufPayload::LoadRouterFromFile(FString path)
{
	return CommandRouterMap::instance().load(TCHAR_TO_UTF8(*path));
}

void UOhtoAiProtobufPayload::SerializeUserLoginPayload(FString username, FString password, TArray<uint8>& payload)
{
	using pb::socket::UserLogin;
	UserLogin userLogin;
	userLogin.set_username(TCHAR_TO_UTF8(*username));
	userLogin.set_password(TCHAR_TO_UTF8(*password));

	MessageWrapper wrapper;
	wrapper.setPayload(userLogin);
	wrapper.setRouter(CommandRouter{ 15, 0 });
	//wrapper.setCmdCode(1);
	auto buffer = wrapper.serializeAsString();
	payload = TArray<uint8>{ reinterpret_cast<const uint8*>(buffer.data()), static_cast<int32>(buffer.length()) };
}

void UOhtoAiProtobufPayload::ParseUserInfoPayload(const TArray<uint8>& Payload, FString& username, int64& id, FString& jwt)
{
	MessageWrapper wrapper;
	wrapper.parseFromString(std::string(reinterpret_cast<const char*>(Payload.GetData()), Payload.Num()));
	auto userInfo = wrapper.object<pb::socket::UserInfo>();

	username = UTF8_TO_TCHAR(userInfo.name().c_str());
	jwt = UTF8_TO_TCHAR(userInfo.jwt().c_str());
	id = userInfo.id();
}

void UOhtoAiProtobufPayload::DumpHexFromArray(const TArray<uint8>& Bytes, FString& String)
{
	String.Empty();
	for (auto& byte : Bytes)
	{
		String += FString::Printf(TEXT("%02X"), byte);
	}
}

void UOhtoAiProtobufPayload::Base64Encode(const TArray<uint8>& Bytes, FString& String)
{
	String = FBase64::Encode(Bytes);
}

void UOhtoAiProtobufPayload::Base64Decode(const FString& String, TArray<uint8>& Bytes)
{
	FBase64::Decode(String, Bytes);
}
