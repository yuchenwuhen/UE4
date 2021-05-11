// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CoopGame/CoopGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SWeapon.h"
#include "SHealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	springArm->bUsePawnControlRotation = true;
	springArm->SetupAttachment(RootComponent);

	GetCapsuleComponent()->SetCollisionResponseToChannel(CHANNEL_WEAPON, ECR_Ignore);

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	cameraCmp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraCmp"));
	cameraCmp->SetupAttachment(springArm);

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("healthComp"));



	zoomFVO = 20.0f;
	zoomSpeed = 20.0f;

	WeaponSocket = "WeaponSocket";
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	defaultFVO = cameraCmp->FieldOfView;
	HealthComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);

	if (HasAuthority())
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurWeapon = GetWorld()->SpawnActor<ASWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
		if (CurWeapon)
		{
			CurWeapon->SetOwner(this);
			CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		}
	}


}

void ASCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}

void ASCharacter::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

void ASCharacter::CrouchOn()
{
	Crouch();
}

void ASCharacter::UnCrouchOn()
{
	UnCrouch();
}

void ASCharacter::ZoomOn()
{
	bWantToZoom = true;
}

void ASCharacter::ZoomUp()
{
	bWantToZoom = false;
}

void ASCharacter::StartFire()
{
	if (CurWeapon)
	{
		CurWeapon->StartFire();
	}
}

void ASCharacter::StopFire()
{
	if (CurWeapon)
	{
		CurWeapon->StopFire();
	}
}

void ASCharacter::OnHealthChanged(USHealthComponent* HealthComp1,float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		//Died
		bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(10.0f);
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFVO = bWantToZoom ? zoomFVO : defaultFVO;
	cameraCmp->SetFieldOfView(FMath::FInterpTo(cameraCmp->FieldOfView, TargetFVO, DeltaTime, zoomSpeed));
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp",this,&ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn",this,&ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&ASCharacter::CrouchOn);
	PlayerInputComponent->BindAction("Crouch",IE_Released,this,&ASCharacter::UnCrouchOn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed,this,&ASCharacter::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::ZoomOn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::ZoomUp);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::StopFire);

}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (cameraCmp)
	{
		return cameraCmp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void ASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacter, CurWeapon);
	DOREPLIFETIME(ASCharacter, bDied);
}

