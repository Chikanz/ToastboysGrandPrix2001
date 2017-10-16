// Fill out your copyright notice in the Description page of Project Settings.

#include "Toasty.h"
#include "BatteryPickup.h"
#include "UnrealNetwork.h"


ABatteryPickup::ABatteryPickup()
{
	bReplicateMovement = true;

	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	//GetStaticMeshComponent()->SetSimulatePhysics(true);

	BatteryPower = 200;
}

void ABatteryPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABatteryPickup, BatteryPower);
}

void ABatteryPickup::PickedUpBy(APawn * Pawn)
{
	Super::PickedUpBy(Pawn);

	if (Role == ROLE_Authority)
	{
		SetLifeSpan(0.1f); //Give leeway time to kill
	}
}
