// Fill out your copyright notice in the Description page of Project Settings.

#include "Toasty.h"
#include "Pickup.h"
#include "net/UnrealNetwork.h"


APickup::APickup()
{
	//Replicate me
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	
	GetStaticMeshComponent()->bGenerateOverlapEvents = true;

	if (Role == ROLE_Authority)
	{
		bIsActive = true;
	}
}

void APickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickup, bIsActive);
}

void APickup::SetActive(bool active)
{
	if (Role == ROLE_Authority)
	{
		bIsActive = active;
	}
}

void APickup::WasCollected_Implementation()
{
	UE_LOG(LogClass, Log, TEXT("ayyyeee"), *GetName());
}

void APickup::OnRep_IsActive()
{

}

void APickup::PickedUpBy(APawn * Pawn)
{
	if (Role == ROLE_Authority)
	{
		PickupInstigator = Pawn;
		ClientOnPickedUpBy(Pawn); //Notify clients
	}
}

void APickup::ClientOnPickedUpBy_Implementation(APawn* Pawn)
{
	PickupInstigator = Pawn;
	WasCollected();
}