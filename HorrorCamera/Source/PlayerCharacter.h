// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SEVENTHPLANET_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

	public:
        APlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float NormalVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
    float NormalSpringArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
    float AimingSpringArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
    float ZoomSmoothness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
    bool bIsAiming;

	protected:
        virtual void BeginPlay() override;

	void AlignPawnToCameraDir();

	void SetSprintVelocity();

	void SetNormalVelocity();

	void StartAimingWeapon();

	void StopAimingWeapon();

	float ForwardValue;
	float RightValue;
	float DesiredSpringArmLength;
};
