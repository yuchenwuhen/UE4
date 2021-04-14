// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,

	Suspicious,

	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mantra")
		class UPawnSensingComponent* PawnSensingComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly,Category = "AI")
	bool bPatrol;

	UFUNCTION()
	void OnPawnSensing(APawn * pawn);

	UFUNCTION()
	void OnNoiseHear(APawn* pawn, const FVector& Location, float Volume);

	FRotator StartRotator;

	FTimerHandle handle;

	UFUNCTION()
	void ResetRotator();

	EAIState GuaradState;

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent,Category="AI")
	void OnStateChanged(EAIState NewState);

	UPROPERTY(EditInstanceOnly,Category = "points")
	AActor * AIPoint1;
	UPROPERTY(EditInstanceOnly, Category = "points")
	AActor * AIPoint2;

	AActor * CurrentPoint;

	void Patrol();

	bool changePoint;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
