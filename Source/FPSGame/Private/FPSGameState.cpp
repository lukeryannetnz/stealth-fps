// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "FPSPlayerController.h"

void AFPSGameState::MulticastOnMissionCompleted_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
    for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
    {
        AFPSPlayerController* PC = Cast<AFPSPlayerController>(It->Get());

        // this function runs on the client and server, so check that this is local
        if(PC && PC->IsLocalController())
        {
            // show the overlay on the hud
            PC->OnMissionCompleted(InstigatorPawn, bMissionSuccess);

            // disable the input
            APawn* Pawn = PC->GetPawn();
            if(Pawn)
            {
                Pawn->DisableInput(PC);
            }
        }
    }
}