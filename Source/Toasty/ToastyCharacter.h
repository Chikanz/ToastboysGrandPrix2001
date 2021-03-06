// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "ToastyCharacter.generated.h"

UCLASS(config=Game)
class AToastyCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Collecty boys */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Battery, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollectionSphere;
public:
	AToastyCharacter();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	float GetInitialPower();	

	UFUNCTION(BlueprintCallable, Category = "Power")
	float GetCurrentPower();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power")
	float MaxPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power")
	float BaseSpeed = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Power")
	int CheckPoints;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Power")
	void UpdatePower(float powerdelta);

	UFUNCTION(BlueprintCallable, Category = "Race")
	void WhenDestroyed();

	UFUNCTION(BlueprintCallable, Category = "Race")
		void Flush();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Power")
		bool isAlive = true;


protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void CollectPickups();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerCollectPickups();

	bool ServerCollectPickups_Validate() { return true; }

	void ServerCollectPickups_Implementation();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float InitialPower;

	virtual void Tick(float dt);

	UPROPERTY()
	FVector CheckpointPos;

	

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class USphereComponent* GetSphere() const { return CollectionSphere; }

private:
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Battery")
	float CollectionSphereRadius;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Power")
	float CurrentPower;
};

