// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();

public:	

	UPROPERTY(VisibleAnywhere,Category="components")
	UBoxComponent* OverLaunch;

	UPROPERTY(VisibleAnywhere, Category = "components")
	UStaticMeshComponent* meshComp;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditInstanceOnly,Category="LaunchPad")
	float LaunchPadAngle;

	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
	float LaunchStrength;

	UPROPERTY(EditDefaultsOnly,Category="LaunchPad")
	UParticleSystem * emitParticle;

};
