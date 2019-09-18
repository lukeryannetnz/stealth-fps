#include "FPSLaunchpadActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.h"

// Sets default values
AFPSLaunchpadActor::AFPSLaunchpadActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if(Materials.Num() > 0)
	{
		for(int i = 0; i < Materials.Num(); i++)
		{
			MeshComp->SetMaterial(i, Materials[i]);
		}
	}

    RootComponent = MeshComp;

	LauncherSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("LauncherSphereComp"));
    LauncherSphereComp->SetGenerateOverlapEvents(true);
    LauncherSphereComp->SetSphereRadius(50);
    LauncherSphereComp->SetupAttachment(MeshComp);
    LauncherSphereComp->OnComponentBeginOverlap.AddDynamic(this, & AFPSLaunchpadActor::LauncherSphereBeginOverlap);
}

// Called when the game starts or when spawned
void AFPSLaunchpadActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSLaunchpadActor::LauncherSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapped with launchpad"));
	AFPSCharacter* Character = Cast<AFPSCharacter>(OtherActor);
	
	if(Character != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully cast OtherActor to AFPSCharacter"));

		Character->LaunchCharacter(FVector(0,0,1000), false, false);
	}
}
