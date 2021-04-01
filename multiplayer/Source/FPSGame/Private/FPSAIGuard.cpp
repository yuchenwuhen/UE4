// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSensing);

}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHear);

	StartRotator = GetActorRotation();
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

		FVector Direction = Location - GetActorLocation();
		Direction.Normalize();
		FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();

		NewLookAt.Roll = 0.0f;
		NewLookAt.Pitch = 0.0f;

		SetActorRotation(NewLookAt);

		GetWorldTimerManager().ClearTimer(handle);

		GetWorldTimerManager().SetTimer(handle,this, &AFPSAIGuard::ResetRotator, 3, false);
	}
}

void AFPSAIGuard::ResetRotator()
{
	SetActorRotation(StartRotator);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

