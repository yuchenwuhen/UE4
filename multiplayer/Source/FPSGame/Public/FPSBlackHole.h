// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSBlackHole.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class FPSGAME_API AFPSBlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSBlackHole();
	UPROPERTY(VisibleAnywhere,Category = "Components")
	UStaticMeshComponent* meshComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* interSphere;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* outerSphere;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor*otherActor, UPrimitiveComponent*otherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};