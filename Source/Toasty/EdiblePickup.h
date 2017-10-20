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
	void PickedUpBy(APawn* Pawn) override;
	AEdiblePickup();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAAA")
	class UParticleSystem* particles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAAA")
	FLinearColor ParticleCol; 

private:
	UParticleSystemComponent* SpawnedParticleSystem;
};
