// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SWeapon.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	UCameraComponent* cameraCmp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USpringArmComponent* springArm;

	void MoveForward(float value);
	void MoveRight(float value);

	void CrouchOn();
	void UnCrouchOn();

	void ZoomOn();
	void ZoomUp();

	bool bWantToZoom;

	UPROPERTY(EditDefaultsOnly,Category="FVO")
	float defaultFVO;
	UPROPERTY(EditDefaultsOnly, Category = "FVO")
	float zoomFVO;
	UPROPERTY(EditDefaultsOnly, Category = "FVO", meta = (ClampMin = 0.1f, ClampMax = 100.0f))
	float zoomSpeed;

	ASWeapon* CurWeapon;

	FName WeaponSocket;

	UPROPERTY(EditDefaultsOnly,Category = "Player")
	TSubclassOf<ASWeapon> WeaponClass;

	void StartFire();

	void StopFire();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual FVector GetPawnViewLocation() const override;

};
