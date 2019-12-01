// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AFPSProjectile::AFPSProjectile() { // Use a sphere as a simple collision representation
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp -> InitSphereRadius(5.0f);
    CollisionComp -> SetCollisionProfileName("Projectile");
    CollisionComp -> OnComponentHit.AddDynamic(this, & AFPSProjectile::OnHit);
    // set up a notification for when this component hits something blocking

    // Players can't walk on it
    CollisionComp -> SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
    CollisionComp -> CanCharacterStepUpOn = ECB_No;

    // Set as root component
    RootComponent = CollisionComp;

    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement -> UpdatedComponent = CollisionComp;
    ProjectileMovement -> InitialSpeed = 3000.0f;
    ProjectileMovement -> MaxSpeed = 3000.0f;
    ProjectileMovement -> bRotationFollowsVelocity = true;
    ProjectileMovement -> bShouldBounce = true;

    // Die after 3 seconds by default
    InitialLifeSpan = 3.0f;

    // replicate from server to client for multiplayer.
    SetReplicates(true);
    SetReplicateMovement(true);
}


void AFPSProjectile::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit) { // Only add impulse and destroy projectile if we hit a physics
    if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp -> IsSimulatingPhysics()) {
        OtherComp -> AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
    }

    // only run this code on the server as that is where the AI logic runs
    if(Role == ROLE_Authority)
    {
        MakeNoise(1.0f, Instigator);
        Destroy();
    }
}
