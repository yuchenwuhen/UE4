// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{

	OverLaunch = CreateDefaultSubobject<UBoxComponent>(TEXT("LaunchPad"));
	OverLaunch->SetBoxExtent(FVector(75, 75, 50));
	RootComponent = OverLaunch;

	OverLaunch->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::BeginOverlap);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetupAttachment(OverLaunch);

	LaunchPadAngle = 45.0f;
	LaunchStrength = 1500.0f;

}

void AFPSLaunchPad::BeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	FRotator local = GetActorRotation();
	local.Pitch += LaunchPadAngle;
	FVector velocity = local.Vector() * LaunchStrength;

	ACharacter * player = Cast<ACharacter>(OtherActor);

	if (player)
	{
		player->LaunchCharacter(velocity, true, true);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), emitParticle, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(velocity, NAME_None, true);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), emitParticle, GetActorLocation());
	}
}

