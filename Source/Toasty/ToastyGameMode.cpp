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

	//Set HUDy boy
	static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDClass(TEXT("/Game/Blueprints/BP_ToastyHUD"));
	if (PlayerHUDClass.Class != NULL)
	{
		HUDClass = PlayerHUDClass.Class;
	}

	DecayRate = 0.02f;

	PowerDrainDelay = 0.25f;
}

void AToastyGameMode::BeginPlay()
{
	GetWorldTimerManager().SetTimer(PowerDrainTimer, this, &AToastyGameMode::DrainPowerOverTime, PowerDrainDelay, true);
}

void AToastyGameMode::DrainPowerOverTime()
{
	UWorld* worl = GetWorld();
	check(worl);

	for (FConstControllerIterator It = worl->GetControllerIterator(); It; ++It)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(*It))
		{
			if (AToastyCharacter* BatteryCharacter = Cast<AToastyCharacter>(PlayerController->GetPawn()))
			{
				if (BatteryCharacter->GetCurrentPower() > 0)
				{
					BatteryCharacter->UpdatePower(-PowerDrainDelay * DecayRate * (BatteryCharacter->GetInitialPower()));
				}
			}
		}
	}
}
