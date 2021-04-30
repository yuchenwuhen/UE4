// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CoopGame/CoopGame.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("COOP.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines for Weapons"),
	ECVF_Cheat
);

// Sets default values
ASWeapon::ASWeapon()
{

	skeletaMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("skeletaMeshComp"));
	RootComponent = skeletaMeshComp;

	MuzzleEffectSocket = "MuzzleEffect";

	DamageBase = 20.0f;

	RateOfFire = 600;
}


void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShoot = 60 / RateOfFire;
}


void ASWeapon::Fire()
{
	AActor* owner = GetOwner();
	if (owner)
	{
		FVector start;
		FRotator rotator;
		owner->GetActorEyesViewPoint(start, rotator);
		FVector end;
		end = (rotator.Vector() * 10000) + start;

		FCollisionQueryParams params;
		params.AddIgnoredActor(owner);
		params.AddIgnoredActor(this);
		params.bTraceComplex = true;
		params.bReturnPhysicalMaterial = true;

		FHitResult result;

		FVector trailEnd = end;

		if (GetWorld()->LineTraceSingleByChannel(result, start, end, CHANNEL_WEAPON, params))
		{
			EPhysicalSurface surface = UGameplayStatics::GetSurfaceType(result);

			float damage = DamageBase;

			if (surface == SURFACE_FLESHVOLNERABLE)
			{
				damage = DamageBase * 2.5f;
			}

			UGameplayStatics::ApplyPointDamage(result.GetActor(), damage, rotator.Vector(), result, owner->GetInstigatorController(), this, damageType);
			
			UParticleSystem* selectParticle = nullptr;

			switch (surface)
			{
			case SURFACE_FLESHDEFAULT:
			case SURFACE_FLESHVOLNERABLE:
				selectParticle = FleshVulnerableEffect;
				break;
			default:
				selectParticle = FleshDefaultEffect;
				break;
			}

			if (selectParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), selectParticle, result.ImpactPoint, result.ImpactNormal.Rotation());
			}

			trailEnd = result.ImpactPoint;
		}

		PlayFireEffects(start,trailEnd);

		LastFireTime = GetWorld()->TimeSeconds;
	}

}

void ASWeapon::StartFire()
{
	float delay = LastFireTime + TimeBetweenShoot - GetWorld()->TimeSeconds;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ASWeapon::Fire, TimeBetweenShoot, true, delay);
}

void ASWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void ASWeapon::PlayFireEffects(FVector start, FVector end)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, skeletaMeshComp, MuzzleEffectSocket);
	}

	if (TraceEffect)
	{
		FVector pos = skeletaMeshComp->GetSocketLocation(MuzzleEffectSocket);
		UParticleSystemComponent* particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, pos);
		particle->SetVectorParameter("Target", end);
	}

	if (DebugWeaponDrawing > 0)
	{
		DrawDebugLine(GetWorld(), start, end, FColor::White, false, 1.0f, 0, 1.0f);
	}

	APawn* pawn = Cast<APawn>(GetOwner());
	if (pawn)
	{
		APlayerController* PC = Cast<APlayerController>(pawn->GetController());
		if (PC)
		{
			PC->ClientStartCameraShake(Shake);
		}
	}
}

