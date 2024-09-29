// M4thi4sL  (2024)


#include "StartPoint.h"


// Sets default values
AStartPoint::AStartPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStartPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStartPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

