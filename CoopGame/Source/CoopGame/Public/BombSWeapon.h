// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "BombSWeapon.generated.h"

class SProjectile;

/**
 * 
 */
UCLASS()
class COOPGAME_API ABombSWeapon : public ASWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly,Category="Projectile")
	TSubclassOf<AActor> ProjectileClass;
};
