// Fill out your copyright notice in the Description page of Project Settings.
#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::PawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::NoiseHeard);
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSAIGuard::PawnSeen(APawn* Pawn)
{
	UE_LOG(LogTemp, Log, TEXT("Pawn seen by guard!"));

	if(Pawn == nullptr)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);
}

void AFPSAIGuard::NoiseHeard(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), InstigatorPawn->GetActorLocation(), 32.0f, 12, FColor::Yellow, false, 10.0f);
}