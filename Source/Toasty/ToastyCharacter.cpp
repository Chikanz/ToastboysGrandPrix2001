// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Toasty.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "ToastyCharacter.h"
#include "UnrealNetwork.h"
#include "Pickup.h"
#include "BatteryPickup.h"

//////////////////////////////////////////////////////////////////////////
// AToastyCharacter

AToastyCharacter::AToastyCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CollectionSphereRadius = 200.0f;

	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(CollectionSphereRadius);

	//Detect collision with floor
	//HAHA NOPE
	//GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AToastyCharacter::OnHit);

	//OnDestroyed.AddDynamic(this, &AToastyCharacter::WhenDestroyed);


	InitialPower = 2000;
	CurrentPower = InitialPower;

	PrimaryActorTick.bCanEverTick = true; //We won't be ticked by default  

	//Set default checkpoint
	CheckpointPos = GetActorLocation();

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AToastyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AToastyCharacter, CollectionSphereRadius);
	DOREPLIFETIME(AToastyCharacter, InitialPower);
	DOREPLIFETIME(AToastyCharacter, CurrentPower);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AToastyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AToastyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AToastyCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AToastyCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AToastyCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AToastyCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AToastyCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AToastyCharacter::OnResetVR);

	//Pickyup
	//PlayerInputComponent->BindAction("CollectPickups", IE_Pressed, this, &AToastyCharacter::CollectPickups);
}

void AToastyCharacter::CollectPickups()
{
	ServerCollectPickups_Implementation();
}

void AToastyCharacter::ServerCollectPickups_Implementation()
{
	if (!ServerCollectPickups_Validate()) return;

	float powerCollected = 0.0f;

	if (Role == ROLE_Authority)
	{
		TArray<AActor*> Collected;
		CollectionSphere->GetOverlappingActors(Collected);

		for (int i = 0; i < Collected.Num(); ++i)
		{
			APickup* const TestPickup = Cast<APickup>(Collected[i]);
			if (TestPickup != NULL && !TestPickup->IsPendingKill() && TestPickup->IsActive())
			{
				if (ABatteryPickup* const b = Cast<ABatteryPickup>(TestPickup))
					UpdatePower(b->GetPower());

				TestPickup->PickedUpBy(this);
				TestPickup->SetActive(false);

				//Set Checkpoint
				CheckpointPos = TestPickup->GetActorLocation();
			}
		}
	}
}

void AToastyCharacter::Tick(float dt)
{	
	Super::Tick(dt);
	CollectPickups();
}

void AToastyCharacter::WhenDestroyed()
{
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	TeleportTo(CheckpointPos, Rotation, false, false);
	
	////Laters
	//Controller->UnPossess();

	////Spawn another toasty boy
	//FRotator Rotation(0.0f, 0.0f, 0.0f);
	//FActorSpawnParameters SpawnInfo;
	//auto newMe = GetWorld()->SpawnActor<AToastyCharacter>(CheckpointPos, Rotation, SpawnInfo);

	////Posess new me
	//newMe->Controller->Possess(newMe);

	////Kill
	////Destroy();
}


float AToastyCharacter::GetInitialPower()
{
	return InitialPower;
}

float AToastyCharacter::GetCurrentPower()
{
	return CurrentPower;
}

void AToastyCharacter::UpdatePower(float powerdelta)
{
	if (Role == ROLE_Authority)
	{
		CurrentPower += powerdelta;
	}
}

void AToastyCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AToastyCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AToastyCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AToastyCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AToastyCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AToastyCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AToastyCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
