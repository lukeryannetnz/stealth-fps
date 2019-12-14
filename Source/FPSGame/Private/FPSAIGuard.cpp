// Fill out your copyright notice in the Description page of Project Settings.
#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "AIController.h"
#include "Engine/TargetPoint.h"
#include "Navigation/PathFollowingComponent.h"
#include "net/UnrealNetwork.h"

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
	IsOnPatrol = false;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	AI = Cast<AAIController>(GetController());
	if(AI)
	{
		AI->ReceiveMoveCompleted.AddDynamic(this, &AFPSAIGuard::MoveCompleted);
	}
	Super::BeginPlay();
	BeginPatrol();
}

void AFPSAIGuard::BeginPatrol()
{
	if(IsOnPatrol && AI)
	{
		CurrentPatrolPointIndex = 0;
		AI->MoveToActor(PatrolPoints[CurrentPatrolPointIndex]);
	}
}

void AFPSAIGuard::StopPatrol()
{
	if(AI)
	{
		AI->StopMovement();
	}
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

	if(GuardState == EAIState::Idle)
	{
		AI->MoveToActor(PatrolPoints[CurrentPatrolPointIndex]);
	}
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

///
/// Called on clients (not the server) when the guard state variable is changed.
///
void AFPSAIGuard::OnRep_GuardState()
{
	// trigger blueprint animation
	OnGuardStateChange(GuardState);
};

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if(NewState == GuardState)
	{
		return;
	}

	GuardState = NewState;

	// This function is likely running on a server so replication won't trigger automatically, so call it to
	// trigger blueprint animation in the case that the server is also a client.
	OnRep_GuardState();
}

///
/// Set up replication of properties from server to client.
///
void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// replicate GuardState
	DOREPLIFETIME(AFPSAIGuard, GuardState);
}
