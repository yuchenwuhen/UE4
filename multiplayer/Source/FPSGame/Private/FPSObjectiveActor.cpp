// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "FPSCharacter.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = meshComp;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	//ÉèÖÃÅö×²Îª²éÑ¯£¨ÉäÏß¼ì²â£¬É¨Ãè£¬ÖØµþ£©
	sphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	sphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	sphereComp->SetupAttachment(meshComp);
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();

	PlayEffects();
}

void AFPSObjectiveActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickUpFx, GetActorLocation());
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();

	AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
	if (MyCharacter)
	{
		MyCharacter->bIsCarryingObjective = true;

		Destroy();
	}
}

