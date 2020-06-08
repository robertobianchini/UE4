// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(32.0F, 100.0F);

	BaseTurnRate = 45.F;
	BaseLookUpRate = 45.F;
	ForwardValue = 0.0F;
	RightValue = 0.0F;

	bIsAiming = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* move = GetCharacterMovement();
	move->bOrientRotationToMovement = true;
	move->RotationRate = FRotator(0.0F, 540.0F, 0.0F);
	move->JumpZVelocity = 600.F;
	move->AirControl = 0.2F;
	move->bUseControllerDesiredRotation = false;
	move->bOrientRotationToMovement = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 135.0F;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()-> MaxWalkSpeed = NormalVelocity;
	CameraBoom->TargetArmLength = NormalSpringArmLength;
	DesiredSpringArmLength = NormalSpringArmLength;
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::SetSprintVelocity);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::SetNormalVelocity);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::StartAimingWeapon);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::StopAimingWeapon);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AlignPawnToCameraDir();

	if (!FMath::IsNearlyEqual(CameraBoom->TargetArmLength, DesiredSpringArmLength, 0.5F))
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(
            CameraBoom->TargetArmLength,
            DesiredSpringArmLength,
            DeltaTime,
            ZoomSmoothness);
	}
}


void APlayerCharacter::MoveForward(float Value)
{
	ForwardValue = Value;
	if ((Controller != nullptr) && (Value != 0.0F))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}


void APlayerCharacter::MoveRight(float Value)
{
	RightValue = Value;
	if ((Controller != nullptr) && (Value != 0.0F))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}


void APlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}


void APlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void APlayerCharacter::AlignPawnToCameraDir()
{
	bool use = ForwardValue != 0.0F || RightValue != 0.0F || bIsAiming;
	GetCharacterMovement()->bUseControllerDesiredRotation = use;
}


void APlayerCharacter::SetSprintVelocity()
{
	auto move = GetCharacterMovement();

	if (move != nullptr)
	{
		if (bIsAiming)
		{
			if (move->MaxWalkSpeed != NormalVelocity) move->MaxWalkSpeed = NormalVelocity;

			return;
		}
		move->MaxWalkSpeed = SprintVelocity;
	}
}


void APlayerCharacter::SetNormalVelocity()
{
	auto move = GetCharacterMovement();
	if (move != nullptr) move->MaxWalkSpeed = NormalVelocity;
}


void APlayerCharacter::StartAimingWeapon()
{
	bIsAiming = true;
	SetNormalVelocity();
	DesiredSpringArmLength = AimingSpringArmLength;
}


void APlayerCharacter::StopAimingWeapon()
{
	bIsAiming = false;
	DesiredSpringArmLength = NormalSpringArmLength;
}
