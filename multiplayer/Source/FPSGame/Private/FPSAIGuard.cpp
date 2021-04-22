// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Net/UnrealNetwork.h"

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

	GuaradState = EAIState::Idle;

	changePoint = true;
	if (bPatrol)
	{
		Patrol();
	}
}

void AFPSAIGuard::OnPawnSensing(APawn * pawn)
{
	if (pawn)
	{
		DrawDebugSphere(GetWorld(), pawn->GetActorLocation(), 35, 12, FColor::Red, false, 5);

		//task fail
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->CompeleteMission(pawn, false);
		}
	}

	SetGuardState(EAIState::Alerted);

	AController * controller = GetController();
	if (controller)
	{
		controller->StopMovement();
	}
}

void AFPSAIGuard::OnNoiseHear(APawn* pawn, const FVector& Location, float Volume)
{
	if (GuaradState == EAIState::Alerted)
	{
		return;
	}

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

	SetGuardState(EAIState::Suspicious);

	AController * controller = GetController();
	if (controller)
	{
		controller->StopMovement();
	}
}

void AFPSAIGuard::ResetRotator()
{
	if (GuaradState == EAIState::Alerted)
	{
		return;
	}

	SetActorRotation(StartRotator);

	SetGuardState(EAIState::Idle);

	if (bPatrol)
	{
		Patrol();
	}
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuaradState);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuaradState == NewState)
	{
		return;
	}

	GuaradState = NewState;

	//·þÎñÆ÷Ö´ÐÐ
	OnRep_GuardState();

}

void AFPSAIGuard::Patrol()
{
	if (CurrentPoint == nullptr || CurrentPoint == AIPoint2)
	{
		CurrentPoint = AIPoint1;
	}
	else
	{
		CurrentPoint = AIPoint2;
	}
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPoint);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPoint)
	{
		FVector delta = GetActorLocation() - CurrentPoint->GetActorLocation();
		float dis = delta.Size();
		if (dis < 50)
		{
			Patrol();
		}
	}
	if (GetDistanceTo(AIPoint1) < 0.01f && !changePoint)
	{
		changePoint = true;
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), AIPoint2);
	}
	else if(GetDistanceTo(AIPoint2) < 0.01f && changePoint)
	{
		changePoint = true;
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), AIPoint1);
	}

}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuaradState);
}

