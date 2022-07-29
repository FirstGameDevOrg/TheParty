// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThePartyGameMode.h"
#include "ThePartyPlayerController.h"
#include "ThePartyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThePartyGameMode::AThePartyGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AThePartyPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}