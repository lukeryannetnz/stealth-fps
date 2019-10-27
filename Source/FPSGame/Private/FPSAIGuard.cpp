// Fill out your copyright notice in the Description page of Project Settings.
#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "AIController.h"
#include "Engine/TargetPoint.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::PawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::NoiseHeard);

	OriginalRotation = GetActorRotation();

	GuardState = EAIState::Idle;

	AAIController* AI = Cast<AAIController>(GetController());
	AI->ReceiveMoveCompleted.AddDynamic(this, &AFPSAIGuard::MoveCompleted);
	IsOnPatrol = false;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	BeginPatrol();
}

void AFPSAIGuard::BeginPatrol()
{
	if(IsOnPatrol)
	{
		CurrentPatrolPointIndex = 0;
		AI->MoveToActor(PatrolPoints[CurrentPatrolPointIndex]);
	}
}

void AFPSAIGuard::StopPatrol()
{
	AI->StopMovement();
}

void AFPSAIGuard::MoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if(CurrentPatrolPointIndex >= PatrolPoints.Num() - 1)
	{
		CurrentPatrolPointIndex = 0;
	}
	else
	{
		CurrentPatrolPointIndex++;
	}
	
	AI->MoveToActor(PatrolPoints[CurrentPatrolPointIndex]);
}

void AFPSAIGuard::PawnSeen(APawn* Pawn)
{
	UE_LOG(LogTemp, Log, TEXT("Pawn seen by guard!"));
	SetGuardState(EAIState::Alerted);

	if(Pawn == nullptr)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	AFPSGameMode* Gm = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());

	if(Gm)
	{
		Gm->CompleteMission(Pawn, false);
	}
	StopPatrol();
}

void AFPSAIGuard::NoiseHeard(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
	if(GuardState == EAIState::Alerted)
	{
		return;
	}

	SetGuardState(EAIState::Suspicious);

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Yellow, false, 10.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	
	FRotator NewDirection = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewDirection.Pitch = 0.0f;
	NewDirection.Roll = 0.0f;

	SetActorRotation(NewDirection);

	GetWorldTimerManager().ClearTimer(ResetRotationTimerHandle);
	GetWorldTimerManager().SetTimer(ResetRotationTimerHandle, this, &AFPSAIGuard::ResetRotation, 3.0f);
	StopPatrol();
}

void AFPSAIGuard::ResetRotation()
{
	if(GuardState == EAIState::Alerted)
	{
		return;
	}

	SetGuardState(EAIState::Idle);
	SetActorRotation(OriginalRotation);
	BeginPatrol();
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if(NewState == GuardState)
	{
		return;
	}

	GuardState = NewState;

	OnGuardStateChange(GuardState);
}