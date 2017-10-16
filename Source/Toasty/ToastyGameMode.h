// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "ToastyGameMode.generated.h"

UCLASS(minimalapi)
class AToastyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AToastyGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Power")
		float GetDecayRate() {return DecayRate;}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Power")
	float PowerDrainDelay;

	FTimerHandle PowerDrainTimer;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float DecayRate;

private:
	void DrainPowerOverTime();
};



