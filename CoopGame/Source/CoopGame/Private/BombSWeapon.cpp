// Fill out your copyright notice in the Description page of Project Settings.


#include "BombSWeapon.h"
#include "SProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"


void ABombSWeapon::Fire()
{
	AActor* owner = GetOwner();
	if (owner && ProjectileClass)
	{
		FVector start;
		FRotator rotator;
		owner->GetActorEyesViewPoint(start, rotator);

		FVector muzzlePos = skeletaMeshComp->GetSocketLocation(MuzzleEffectSocket);
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* projectile =GetWorld()->SpawnActor<AActor>(ProjectileClass, muzzlePos, rotator, params);

	}
}
