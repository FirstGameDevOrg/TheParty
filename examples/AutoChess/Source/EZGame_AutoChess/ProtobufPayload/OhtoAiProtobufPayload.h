// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OhtoAiProtobufPayload.generated.h"

/**
 * 
 */
UCLASS()
class EZGAME_AUTOCHESS_API UOhtoAiProtobufPayload : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "OhtoAi | ProtobufPayload | Router")
		static bool LoadRouterFromFile(FString path);
	
	UFUNCTION(BlueprintCallable, Category = "OhtoAi | ProtobufPayload | Login")
		static void SerializeUserLoginPayload(FString username, FString password, TArray<uint8>& Payload);

	UFUNCTION(BlueprintCallable, Category = "OhtoAi | ProtobufPayload | Login")
		static void ParseUserInfoPayload(const TArray<uint8>& Payload, FString& username, int64& id, FString& jwt);

	UFUNCTION(BlueprintCallable, Category = "OhtoAi | ProtobufPayload")
		static void GetRouterFromPayloadClass(const FString& PayloadClass, int32& cmdCode);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "OhtoAi | ProtobufPayload")
		static void MergeRouter(int high, int low, int32& cmdCode);

	UFUNCTION(BlueprintCallable, Category = "OhtoAi | ProtobufPayload")
		static void ParseExternalMessage(const TArray<uint8>& Payload, int32& cmdCode, int32& protocolSwitch, int32& cmdMerge, int32& responseStatus, FString& validMsg, TArray<uint8>&data);

	UFUNCTION(BlueprintCallable, Category = "OhtoAi | ProtobufPayload | Register")
		static void SerializeUserRegisterPayload(FString mail, FString username, FString password, TArray<uint8>& Payload);
	
	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "Hex Encode"), Category = "OhtoAi | ProtobufPayload | Tool")
		static void HexEncode(const TArray<uint8>& Bytes, FString& String);

	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "Base64 Encode"), Category = "OhtoAi | ProtobufPayload | Tool")
		static void Base64Encode(const TArray<uint8>& Bytes, FString& String);

	UFUNCTION(BlueprintCallable, meta = (CompactNodeTitle = "Base64 Decode"), Category = "OhtoAi | ProtobufPayload | Tool")
		static void Base64Decode(const FString& String, TArray<uint8>& Bytes);
};
