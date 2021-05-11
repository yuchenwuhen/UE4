// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UCameraShakeBase;

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	UENUM(BlueprintType, Category = "")
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
	FVector_NetQuantize TraceTo;

	UPROPERTY()
	int index;
};

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();


	UFUNCTION(BlueprintCallable,Category="Weapon")
	virtual void Fire();

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerFire();

	void StartFire();
	void StopFire();

	FTimerHandle FireTimerHandle;
	float RateOfFire;

	float LastFireTime;

	float TimeBetweenShoot;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> damageType;

	void PlayFireEffects(FVector end);

	void PlayImpactEffect(EPhysicalSurface surfaceType, FVector impactPoint);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf <UCameraShakeBase > Shake;

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	USkeletalMeshComponent* skeletaMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* FleshDefaultEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* FleshVulnerableEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* MuzzleEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TraceEffect;

	FName MuzzleEffectSocket;

	UPROPERTY(EditDefaultsOnly,Category="Weapon")
	float DamageBase;

	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	UFUNCTION()
	void OnRep_HitScanTrace();

};
