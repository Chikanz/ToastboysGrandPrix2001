// Fill out your copyright notice in the Description page of Project Settings.

#include "Toasty.h"
#include "EdiblePickup.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"


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

void AEdiblePickup::BeginPlay()
{	
	UWorld* World = GetWorld();

	if (World)
	{		
		SpawnedParticleSystem =
			UGameplayStatics::SpawnEmitterAtLocation(World, particles, GetActorLocation(), FRotator::ZeroRotator, false);		

		if (SpawnedParticleSystem)
		{
			SpawnedParticleSystem->SetColorParameter("InstanceColorScaleOverLife", ParticleCol);
		}
	}
}
