// 2014 - 2017 Soverance Studios
// http://www.soverance.com

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "EndlessReachHD.h"
#include "PickupMaster.h"

// Sets default values 
APickupMaster::APickupMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Curves/MagnetCurve.MagnetCurve"));
	MagnetCurve = Curve.Object;
	MagnetTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MagnetTimeline"));
	InterpFunction.BindUFunction(this, FName{ TEXT("TimelineFloatReturn") });
}

// Called when the game starts or when spawned
void APickupMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupMaster::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// We're not currently using this function...
}

void APickupMaster::OnOverlap(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		AEndlessReachHDPawn* HitPlayer = Cast<AEndlessReachHDPawn>(OtherActor);  // Check if hit actor is the player

		// Proceed with damage functions if you hit the player
		if (HitPlayer)
		{
			Player = HitPlayer;  // Store reference to player within this instance

			if (OtherComp == Player->ShipMeshComponent)  // if the overlapping component is the Player's Ship
			{
				if (!bPickedUp)  // if this pickup is not yet picked up
				{
					bPickedUp = true;  // flag picked up
					OnPlayerCollision.Broadcast();  // broadcast collision event
				}
			}
			if (OtherComp == Player->MagnetRadius)  // if the overlapping component is the Magnet Radius
			{
				if (Player->bMagnetEnabled)  // if the magnet is enabled
				{
					MagnetTimeline->AddInterpFloat(MagnetCurve, InterpFunction, FName{ TEXT("Float") });
					MagnetTimeline->PlayFromStart();  // start magnetize timeline
				}
			}
		}
	}
}

// Destroy this object
void APickupMaster::StartDestruction()
{
	Destroy();
}

// Magnet Timeline
void APickupMaster::TimelineFloatReturn(float val)
{
	if (Player)  // this variable should have been set when initial contact was made with the magnet radius component
	{
		float NewX = FMath::Lerp(FMath::FloorToInt(GetActorLocation().X), FMath::FloorToInt(Player->GetActorLocation().X), val);  // create new X value
		float NewY = FMath::Lerp(FMath::FloorToInt(GetActorLocation().Y), FMath::FloorToInt(Player->GetActorLocation().Y), val);  // create new Y value
		float NewZ = GetActorLocation().Z;  // create new Z value
		// lerp from the pickup's current X/Y location to player's current X/Y location
		RootComponent->SetWorldLocation(FVector(NewX, NewY, NewZ));
	}
}