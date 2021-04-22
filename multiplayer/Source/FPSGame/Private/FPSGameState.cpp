// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "FPSPlayerController.h"

void AFPSGameState::MulticastOnMissionCompelete_Implementation(APawn* InstigatiorPawn, bool bMissionSuccess)
{

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator();It;It++)
	{
		AFPSPlayerController* controller = It->Get();
		if (controller && controller->IsLocalPlayerController())
		{
			controller->OnCompeleteMission(InstigatiorPawn, bMissionSuccess);

			APawn* pawn = controller->GetPawn();
			if (pawn)
			{
				pawn->DisableInput(nullptr);
			}
		}
	}
}
