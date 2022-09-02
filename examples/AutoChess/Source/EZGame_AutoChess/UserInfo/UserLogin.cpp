// Fill out your copyright notice in the Description page of Project Settings.


#include "UserLogin.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"
#include <nlohmann/json.hpp>
#include <cpp-httplib/httplib.h>
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"

// Sets default values
AUserLogin::AUserLogin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

int AUserLogin::UserLogin(FString UserName, FString Password, FString& message)
{
	httplib::Client loginCli("localhost", 8088);

	nlohmann::json j;
	j["username"] = TCHAR_TO_UTF8(&UserName);
	j["password"] = TCHAR_TO_UTF8(&Password);

	auto ret = loginCli.Post("/login", j.dump(), "application/json");
	if (!ret)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot connect to server"));
		return -1;
	}
	if (ret->status == 200)
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Failed"));
	}
	j = nlohmann::json::parse(ret->body);
	message = j["message"].get<std::string>().c_str();
	return ret->status;
}

// Called when the game starts or when spawned
void AUserLogin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUserLogin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

