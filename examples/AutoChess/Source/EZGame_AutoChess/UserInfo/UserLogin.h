// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "../ActorWebSocket.h"
#include "GameFramework/Actor.h"
#include "UserLogin.generated.h"

UCLASS()
class EZGAME_AUTOCHESS_API AUserLogin : public AActorWebSocket
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUserLogin();

	UFUNCTION(BlueprintCallable, Category = "UserLogin")
	bool Login(FString UserName, FString Password);

	//AActorWebSocket* client = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
