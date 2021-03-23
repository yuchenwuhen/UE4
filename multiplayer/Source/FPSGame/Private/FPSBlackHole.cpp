// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = meshComponent;

	interSphere = CreateDefaultSubobject<USphereComponent>(TEXT("interSphere"));
	interSphere->SetSphereRadius(100);
	interSphere->SetupAttachment(meshComponent);

	interSphere->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapInnerSphere);

	outerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("outerSphere"));
	outerSphere->SetSphereRadius(3000);
	outerSphere->SetupAttachment(meshComponent);

}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
}



void AFPSBlackHole::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor*otherActor, UPrimitiveComponent*otherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (otherActor)
	{
		otherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComps;
	outerSphere->GetOverlappingComponents(OverlappingComps);

	for (int32 i = 0;i<OverlappingComps.Num();i++)
	{
		UPrimitiveComponent* primComp = OverlappingComps[i];
		if (primComp && primComp->IsSimulatingPhysics())
		{
			const float SphereRadius = outerSphere->GetScaledSphereRadius();
			const float ForceStrength = -2000;

			primComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}

}

