// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BatteryPickup.h"
#include "EdiblePickup.generated.h"

/**
 * 
 */
UCLASS()
class TOASTY_API AEdiblePickup : public ABatteryPickup
{
	GENERATED_BODY()
	
public:
	virtual void PickedUpBy(APawn* Pawn) override;
	
	
};
