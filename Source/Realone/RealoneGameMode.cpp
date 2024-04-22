// Copyright Epic Games, Inc. All Rights Reserved.

#include "RealoneGameMode.h"
#include "RealoneCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MyGameState.h"


ARealoneGameMode::ARealoneGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	 GameStateClass = AMyGameState::StaticClass();

}
