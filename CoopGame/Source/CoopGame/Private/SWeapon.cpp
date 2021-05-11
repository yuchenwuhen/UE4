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
#include "Net/UnrealNetwork.h"

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

	SetReplicates(true);

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}


void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShoot = 60 / RateOfFire;
}


void ASWeapon::OnRep_HitScanTrace()
{

	PlayFireEffects( HitScanTrace.TraceTo);

	PlayImpactEffect(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

void ASWeapon::Fire()
{
	if (!HasAuthority())
	{
		ServerFire();
	}

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

		EPhysicalSurface surface = EPhysicalSurface::SurfaceType_Default;

		if (GetWorld()->LineTraceSingleByChannel(result, start, end, CHANNEL_WEAPON, params))
		{
			surface = UGameplayStatics::GetSurfaceType(result);

			float damage = DamageBase;

			if (surface == SURFACE_FLESHVOLNERABLE)
			{
				damage = DamageBase * 2.5f;
			}

			UGameplayStatics::ApplyPointDamage(result.GetActor(), damage, rotator.Vector(), result, owner->GetInstigatorController(), this, damageType);
			
			PlayImpactEffect(surface, result.ImpactPoint);

			trailEnd = result.ImpactPoint;

			UE_LOG(LogTemp, Warning, TEXT("hit:%s"), *result.GetActor()->GetFName().ToString());
		}


		if (HasAuthority())
		{

			HitScanTrace.SurfaceType = surface;

			HitScanTrace.TraceTo = trailEnd;

			HitScanTrace.index++;

			if (HasAuthority())
			{
				UE_LOG(LogTemp, Warning, TEXT("Server Update"));
			}
		}


		PlayFireEffects(trailEnd);

		LastFireTime = GetWorld()->TimeSeconds;
	}

}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASWeapon::ServerFire_Validate()
{
	return true;
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

void ASWeapon::PlayFireEffects(FVector end)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, skeletaMeshComp, MuzzleEffectSocket);
	}

	FVector pos = skeletaMeshComp->GetSocketLocation(MuzzleEffectSocket);
	if (TraceEffect)
	{
		
		UParticleSystemComponent* particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, pos);
		if (particle)
		{
			particle->SetVectorParameter("Target", end);
		}
	}

	if (DebugWeaponDrawing > 0)
	{
		DrawDebugLine(GetWorld(), pos, end, FColor::White, false, 1.0f, 0, 1.0f);
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

void ASWeapon::PlayImpactEffect(EPhysicalSurface surfaceType, FVector impactPoint)
{
	UParticleSystem* selectParticle = nullptr;

	switch (surfaceType)
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
		FVector start = skeletaMeshComp->GetSocketLocation(MuzzleEffectSocket);
		FVector Direction = (impactPoint - start);
		Direction.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), selectParticle, impactPoint, Direction.Rotation());
	}
}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASWeapon, HitScanTrace,COND_SkipOwner);
}

