// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "FPSBlackHoleActor.generated.h"

class USphereComponent;
class UPrimitiveComponent;
struct FHitResult;

UCLASS()
class FPSGAME_API AFPSBlackHoleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSBlackHoleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComp;
    
	UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* PhysicsSphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* DestroyerSphereComp;

	UFUNCTION()
	void DestroyerSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
