// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSGameState.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode() { // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    // use our custom HUD class
    HUDClass = AFPSHUD::StaticClass();
	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool IsSuccess) {
	if(InstigatorPawn)
	{
		if(SpectatingViewpointClass)
		{
			AActor* NewViewTarget = nullptr;

			TArray<AActor*> ReturnedActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);

			if(ReturnedActors.Num() > 0)
			{
				NewViewTarget = ReturnedActors[0];

				for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
				{
					APlayerController* PC = It->Get();
					// zoom the camera out on all player controllers
					if(PC)
					{
						// this runs on the server, but this method will be replicated to clients
						PC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SpectatingViewpointClass not set."))
		}
	}

	AFPSGameState* GS = GetGameState<AFPSGameState>();
	if(GS)
	{
		GS->MulticastOnMissionCompleted(InstigatorPawn, IsSuccess);
	}

	OnMissionCompleted(InstigatorPawn, IsSuccess);
}
