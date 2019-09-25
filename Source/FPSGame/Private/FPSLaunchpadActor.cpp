#include "FPSLaunchpadActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.h"

// Sets default values
AFPSLaunchpadActor::AFPSLaunchpadActor()
{
	LaunchComp = CreateDefaultSubobject<UBoxComponent>(TEXT("LaunchComp"));
    LaunchComp->SetGenerateOverlapEvents(true);
	LaunchComp->SetBoxExtent(FVector(75,75,50));
	RootComponent = LaunchComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	LaunchComp->OnComponentBeginOverlap.AddDynamic(this, & AFPSLaunchpadActor::LaunchBeginOverlap);

	LaunchAngle = 35.0f;
	LaunchPower = 1500.0f;
}

void AFPSLaunchpadActor::LaunchBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapped with launchpad"));
	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
	
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch = LaunchAngle;
	FVector LaunchVector = LaunchDirection.Vector() * LaunchPower;

	if(Character != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully cast OtherActor to AFPSCharacter"));

		Character->LaunchCharacter(LaunchVector, true, true);
	}
}
