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

void UOhtoAiProtobufPayload::ParseUserInfo(const TArray<uint8>& data, FString& username, int64& id, FString& jwt)
{
	pb::socket::UserInfo userInfo;
	userInfo.ParseFromArray(data.GetData(), data.Num());

	username = UTF8_TO_TCHAR(userInfo.name().c_str());
	jwt = UTF8_TO_TCHAR(userInfo.jwt().c_str());
	id = userInfo.id();
}


void UOhtoAiProtobufPayload::HasRouter(const FString& PayloadClass, bool& hasRouter)
{
	hasRouter = CommandRouterMap::instance().contains(TCHAR_TO_UTF8(*PayloadClass));
}

void UOhtoAiProtobufPayload::GetRouter(const FString& PayloadClass, int32& cmdCode)
{
	cmdCode = CommandRouterMap::instance().router(TCHAR_TO_UTF8(*PayloadClass), { 0,0 });
}

void UOhtoAiProtobufPayload::MergeRouter(int high, int low, int32& cmdCode)
{
	cmdCode = CommandRouter{ static_cast<int16>(high), static_cast<int16>(low) };
}

void UOhtoAiProtobufPayload::ParseExternalMessage(const TArray<uint8>& Payload, int32& cmdCode, int32& protocolSwitch, int32& cmdMerge, int32& responseStatus, FString& validMsg, TArray<uint8>& data)
{
	MessageWrapper wrapper;
	wrapper.parseFromString(std::string(reinterpret_cast<const char*>(Payload.GetData()), Payload.Num()));
	
	cmdCode = wrapper.cmdCode();
	protocolSwitch = wrapper.protocolSwitch();
	cmdMerge = wrapper.cmdMerge();
	responseStatus = wrapper.responsestatus();
	validMsg = UTF8_TO_TCHAR(wrapper.validMsg().c_str());
	data = TArray<uint8>{ reinterpret_cast<const uint8*>(wrapper.data().data()), static_cast<int32>(wrapper.data().length()) };
}

void UOhtoAiProtobufPayload::HexEncode(const TArray<uint8>& Bytes, FString& String)
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
