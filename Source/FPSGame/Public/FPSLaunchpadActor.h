// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "Engine/EngineTypes.h"

#include "FPSLaunchpadActor.generated.h"

class UBoxComponent;

UCLASS()
class FPSGAME_API AFPSLaunchpadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchpadActor();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* LaunchComp;

	UFUNCTION()
    void LaunchBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
	float LaunchAngle;

	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
	float LaunchPower;
public:	
	
};
