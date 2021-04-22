// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameState.h"


AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompeleteMission(APawn* InstigatorPawn,bool bCompleteMission)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);

		if (bpcamera)
		{
			TArray<AActor*> results;
			UGameplayStatics::GetAllActorsOfClass(this, bpcamera, results);
			if (results.Num() > 0)
			{
				AActor* newTarget = results[0];

				for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
				{
					APlayerController* player = It->Get();
					if (player)
					{
						player->SetViewTargetWithBlend(newTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
					}
				}
				
			}
		}
		
	}

	AFPSGameState* GS = GetGameState<AFPSGameState>();
	if (GS)
	{
		GS->MulticastOnMissionCompelete(InstigatorPawn, bCompleteMission);
	}

	OnCompeleteMission(InstigatorPawn, bCompleteMission);
}
