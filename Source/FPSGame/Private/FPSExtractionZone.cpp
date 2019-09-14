// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone() {
    OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
    OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    OverlapComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    OverlapComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
    OverlapComp->SetBoxExtent(FVector(200.0F));
    RootComponent = OverlapComp;

    OverlapComp->SetHiddenInGame(false);
    OverlapComp->OnComponentBeginOverlap.AddDynamic(this, & AFPSExtractionZone::HandleOverlap);
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f, 200.0f, 200.0f);
	DecalComp->SetupAttachment(RootComponent);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
    UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone"));

	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);

	if(MyPawn == nullptr)
	{
		return;
	}

	if(MyPawn->bIsCarryingObjective)
	{
		AFPSGameMode* Gm = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());

		if(Gm)
		{
			Gm->CompleteMission(MyPawn);
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}
}
