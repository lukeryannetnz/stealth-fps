// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "Engine/EngineTypes.h"

#include "FPSLaunchpadActor.generated.h"

class USphereComponent;

UCLASS()
class FPSGAME_API AFPSLaunchpadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchpadActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent * MeshComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent * LauncherSphereComp;

	UFUNCTION()
    void LauncherSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

     UPROPERTY(EditAnywhere, Category = "Materials")
     TArray<UMaterialInterface*> Materials;
public:	
	
};
