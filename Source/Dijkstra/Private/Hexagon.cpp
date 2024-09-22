// M4thi4sL  (2024)


#include "Hexagon.h"


// Sets default values
AHexagon::AHexagon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AHexagon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHexagon::OnHexClicked(FIntVector HexPosition)
{
	HexClicked.Broadcast(HexPosition);
}
