// Fill out your copyright notice in the Description page of Project Settings.


#include "opendoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
Uopendoor::Uopendoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// ...
}


// Called when the game starts
void Uopendoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	owner = GetOwner();
	//RotateDoor();

	defaultPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void Uopendoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (triggleVolume->IsOverlappingActor(defaultPawn))
	{
		RotateDoor();
	}
	else
	{
		RecoverDoor();
	}
	// ...
}

void Uopendoor::RotateDoor()
{
	FRotator rotate = FRotator(0, 90, 0);
	owner->SetActorRotation(rotate);
}

void Uopendoor::RecoverDoor()
{
	FRotator rotate = FRotator(0, 0, 0);
	owner->SetActorRotation(rotate);
}

