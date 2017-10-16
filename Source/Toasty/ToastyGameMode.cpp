// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Toasty.h"
#include "ToastyGameMode.h"
#include "ToastyCharacter.h"

AToastyGameMode::AToastyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
