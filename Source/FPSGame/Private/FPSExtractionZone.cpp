// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone() {
    OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
    OverlapComp - > SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    OverlapComp - > SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    OverlapComp - > SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
    OverlapComp - > SetBoxExtent(FVector(200.0 F));
    RootComponent = OverlapComp;

    OverlapComp - > SetHiddenInGame(false);
    OverlapComp - > OnComponentBeginOverlap.AddDynamic(this, & AFPSExtractionZone::HandleOverlap);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
    UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone"));
}
