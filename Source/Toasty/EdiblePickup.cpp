// Fill out your copyright notice in the Description page of Project Settings.

#include "Toasty.h"
#include "EdiblePickup.h"

void AEdiblePickup::PickedUpBy(APawn* Pawn)
{
	Super::PickedUpBy(Pawn);

	if (Role == ROLE_Authority)
	{
		SetLifeSpan(0.1f); //kill
	}
}

AEdiblePickup::AEdiblePickup()
{
	isCheckpoint = false;
}


