// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "FPSAIGuard.generated.h"

struct FAIRequestID;
class UPawnSensingComponent;
class ATargetPoint;
class AAIController;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,

	Suspicious,

	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool IsOnPatrol;

	UFUNCTION()
	void PawnSeen(APawn* Pawn);

	UFUNCTION()
	void NoiseHeard(APawn* InstigatorPawn, const FVector& Location, float Volume);

	UFUNCTION()
	void ResetRotation();

	FRotator OriginalRotation;

	FTimerHandle ResetRotationTimerHandle;

	UPROPERTY(ReplcatedUsing=OnRep_GuardState)
	EAIState GuardState;

	UFUNCTION()
	void OnRep_GuardState(); 

	UFUNCTION()
	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnGuardStateChange(EAIState NewState);

	UFUNCTION()
	void BeginPatrol();

	UFUNCTION()
	void MoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UPROPERTY(EditInstanceOnly, Category = "AI")
	TArray<ATargetPoint*> PatrolPoints;

	int CurrentPatrolPointIndex;

	void StopPatrol();

	AAIController* AI;
};
