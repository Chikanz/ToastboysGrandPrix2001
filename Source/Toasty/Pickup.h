// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class TOASTY_API APickup : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	APickup();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category = "Pickup")
		bool IsActive() { return bIsActive; }

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void SetActive(bool active);

	UFUNCTION(BlueprintNativeEvent, Category = "Pickup")
	void WasCollected();
	virtual void WasCollected_Implementation();

	UFUNCTION(BlueprintAuthorityOnly, Category = "Pickup")
	virtual void PickedUpBy(APawn* Pawn);

	virtual void ClientOnPickedUpBy_Implementation(APawn* Pawn);
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_IsActive)
	bool bIsActive;

	//Called when isActive is updated
	UFUNCTION()
	virtual void OnRep_IsActive();

	UPROPERTY(Visibleanywhere, BlueprintReadOnly, Category = "Pickup")
	APawn* PickupInstigator;

private:
	UFUNCTION(NetMulticast, Unreliable)
	void ClientOnPickedUpBy(APawn* Pawn);
};
