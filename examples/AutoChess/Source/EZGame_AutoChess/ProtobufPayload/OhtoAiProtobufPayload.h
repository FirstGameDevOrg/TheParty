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
	UFUNCTION(BlueprintCallable, Category = OhtoAiProtobufPayload)
		static bool LoadRouterFromFile(FString path);
	
	UFUNCTION(BlueprintCallable, Category = OhtoAiProtobufPayload)
		static void SerializeUserLoginPayload(FString username, FString password, TArray<uint8>& Payload);

	UFUNCTION(BlueprintCallable, Category = OhtoAiProtobufPayload)
		static void ParseUserInfoPayload(const TArray<uint8>& Payload, FString& username, int64& id, FString& jwt);

	UFUNCTION(BlueprintCallable, Category = OhtoAiProtobufPayload)
		static void DumpHexFromArray(const TArray<uint8>& Bytes, FString& String);

	UFUNCTION(BlueprintCallable, Category = OhtoAiProtobufPayload)
		static void Base64Encode(const TArray<uint8>& Bytes, FString& String);

	UFUNCTION(BlueprintCallable, Category = OhtoAiProtobufPayload)
		static void Base64Decode(const FString& String, TArray<uint8>& Bytes);
};
