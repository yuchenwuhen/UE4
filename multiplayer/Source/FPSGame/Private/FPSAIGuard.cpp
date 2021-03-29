// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Components"));

	PawnSensing->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSensing);
	PawnSensing->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHear);

}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSAIGuard::OnPawnSensing(APawn * pawn)
{
	if (pawn)
	{
		DrawDebugSphere(GetWorld(), pawn->GetActorLocation(), 35, 12, FColor::Red, false, 5);
	}
}

void AFPSAIGuard::OnNoiseHear(APawn* pawn, const FVector& Location, float Volume)
{
	if (pawn)
	{
		DrawDebugSphere(GetWorld(), Location, 35, 12, FColor::Green, false, 5);
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

