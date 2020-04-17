// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

/**
 * Player controller exists on the client that has the player controller, and the server.
 */
UCLASS()
class FPSGAME_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public: 

	UFUNCTION(BlueprintImplementableEvent, Category="PlayerController")
	void OnMissionCompleted(APawn* InstigatorPawn, bool IsSuccess);
};
