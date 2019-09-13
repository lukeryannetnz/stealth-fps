// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHoleActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h"
#include "Engine/EngineTypes.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
AFPSBlackHoleActor::AFPSBlackHoleActor() { // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootComponent = MeshComp;

    PhysicsSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("PhysicsSphereComp"));
    PhysicsSphereComp->SetupAttachment(MeshComp);

    DestroyerSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("DestroyerSphereComp"));
    DestroyerSphereComp->SetGenerateOverlapEvents(true);
    DestroyerSphereComp->SetSphereRadius(500);
    DestroyerSphereComp->SetupAttachment(MeshComp);
    DestroyerSphereComp->OnComponentBeginOverlap.AddDynamic(this, & AFPSBlackHoleActor::DestroyerSphereBeginOverlap);
}

// Called when the game starts or when spawned
void AFPSBlackHoleActor::BeginPlay() {
    Super::BeginPlay();
}

// Called every frame
void AFPSBlackHoleActor::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    TArray < UPrimitiveComponent *> overlappingComps;
    PhysicsSphereComp->GetOverlappingComponents(overlappingComps);

    for (int i = 0; i < overlappingComps.Num(); i ++) {
        UPrimitiveComponent * overlapper = overlappingComps[i];

        if (overlapper->IsSimulatingPhysics()) {
            overlapper->AddRadialForce(GetActorLocation(), 1500.0F, -2000.0F, RIF_Constant, true);
        }
    }
}

void AFPSBlackHoleActor::DestroyerSphereBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
    if (OtherActor) {
        OtherActor->Destroy();
    }
}
