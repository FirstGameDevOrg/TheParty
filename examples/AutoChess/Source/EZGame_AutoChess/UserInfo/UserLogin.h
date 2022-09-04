// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UserLogin.generated.h"

UCLASS()
class EZGAME_AUTOCHESS_API AUserLogin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUserLogin();

	UFUNCTION(BlueprintCallable, Category = "UserLogin")
	static int UserLogin(FString UserName, FString Password, FString& message);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
