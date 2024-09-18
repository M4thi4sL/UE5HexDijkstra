// Salvatore Spoto 2020


#include "Hexagon.h"

// Sets default values
AHexagon::AHexagon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHexagon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHexagon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

