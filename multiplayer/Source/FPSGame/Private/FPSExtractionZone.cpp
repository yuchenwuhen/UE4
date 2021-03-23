// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{

	zoneComp = CreateDefaultSubobject<UBoxComponent>(TEXT("ZoneComp"));
	zoneComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	zoneComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	zoneComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	zoneComp->SetBoxExtent(FVector(200.0f));

	zoneComp->SetHiddenInGame(false);

	RootComponent = zoneComp;

	zoneComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::BeginOverlap);

	decalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("decalComp"));
	decalComp->DecalSize = FVector(200.0f);
	decalComp->SetupAttachment(zoneComp);
}

void AFPSExtractionZone::BeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AFPSCharacter * actor = Cast<AFPSCharacter>(OtherActor);
	if (actor && actor->bIsCarryingObjective)
	{
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->CompeleteMission(actor);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone."));
}


