// © 2014 - 2017 Soverance Studios
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
#include "Environment/Asteroid.h"

// Sets default values
AAsteroid::AAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Asteroid Body	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> GreyInst(TEXT("/Game/Environment/Materials/Rock/M_Asteroid_Inst.M_Asteroid_Inst"));
	RockColor = GreyInst.Object;
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> RedInst(TEXT("/Game/Environment/Materials/Rock/M_AsteroidHit_Inst.M_AsteroidHit_Inst"));
	RedColor = RedInst.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> AsteroidMesh(TEXT("/Game/Environment/Meshes/Rocks/SM_Cave_Rock_Med_01a.SM_Cave_Rock_Med_01a"));
	AsteroidComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AsteroidBody"));
	RootComponent = AsteroidComponent;
	AsteroidComponent->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	AsteroidComponent->SetStaticMesh(AsteroidMesh.Object);
	AsteroidComponent->SetMaterial(0, RockColor);
	AsteroidComponent->SetSimulatePhysics(true);
	AsteroidComponent->BodyInstance.bLockZTranslation = true;

	HitCount = 0;
}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	OnHitAsteroid.AddDynamic(this, &AAsteroid::HitAsteroid);  // bind asteroid hit function
}

// Called every frame
void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the asteroid is hit with player bullets
void AAsteroid::HitAsteroid()
{
	AsteroidComponent->SetMaterial(0, RedColor);

	if (HitCount < 5)  // if less than 5 hits
	{
		HitCount++;  // increment hit count

		if (HitCount >= 5)  // check if hit count equals 5
		{
			DestroyAsteroid();  // if so, destroy this asteroid
		}
	}
	else
	{
		DestroyAsteroid();  // additional error catch
	}
}

// Called when the asteroid is destroyed
void AAsteroid::DestroyAsteroid()
{
	Destroy();
}