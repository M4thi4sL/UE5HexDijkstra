// M4thi4sL  (2024)


#include "GridManager.h"

#include "StartPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Hexagon.h"

// Sets default values
AGridManager::AGridManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AGridManager::GenerateMap()
{
	ClearGrid();
	GenerateGrid();
	SetGoal();
}

void AGridManager::ClearGrid()
{
	//Find existing hexes and destroy them
	TArray<AActor*> HexActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHexagon::StaticClass(), HexActors);

    for (AActor* Hex : HexActors)
    {
	    Hex->Destroy();
    }

	//find existing startpoints and destroy them
	TArray<AActor*> StartPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStartPoint::StaticClass(), StartPoints);

	for (AActor* Point : StartPoints)
	{
		Point->Destroy();
	}

	//Clear the Hexcell Map
	HexCells.Empty();
}

void AGridManager::GenerateGrid()
{
    for (int Column = 0; Column < Width; ++Column)
	{
		for (int Row = 0; Row <Height; ++Row)
		{
			const FIntVector CubeCoord = OddqToCubeCoordinates(Column, Row);


			const float HexRadius = 100 + Distance;
			float XRow, YRow, XRowShifted;
			bool YRowShifted;
			// Call the CalculateHexPositions function
			CalculateHexPositions(Column, Row, HexRadius, XRow, YRow, XRowShifted, YRowShifted);

			FVector Location;
			if (YRowShifted) Location = FVector(XRow, XRowShifted, 0.0f); // Using XRow and XRowShifted
			else Location = FVector(XRow, YRow, 0.0f); // Using XRow and YRo
			
			AHexagon* Hex = GetWorld()->SpawnActor<AHexagon>(HexagonToSpawn, Location, FRotator(0, 0, 0));
			Hex->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform );
			Hex->HexClicked.AddDynamic(this, &AGridManager::OnTileClicked);
			HexCells.Add(CubeCoord, Hex);

		}
	}
}

void AGridManager::SetGoal()
{
}

FIntVector AGridManager::OddqToCubeCoordinates(int OddqRow, int OddqColumn)
{
	const int x = OddqColumn;
	const int y = x - (OddqColumn * -1);
	const int z  = OddqRow - ((OddqColumn - (OddqColumn & 1)) / 2);
	
	return FIntVector(x,y,z);
}

void AGridManager::CalculateHexPositions(float HexPositionX, float HexPositionY, float HexRadius, 
										 float& XRow, float& YRow, float& XRowShifted, bool& YRowShifted)
{
	// Calculate the base position of the hexagon in the X direction
	XRow = static_cast<int>(HexPositionX) * (1.5f * HexRadius);  // Each hexagon is spaced 1.5 * HexRadius apart

	// Calculate the base position of the hexagon in the Y direction
	YRow = static_cast<int>(HexPositionY) * (HexRadius * FMath::Sqrt(3.0f));  // Height of the hexagon

	// YRowShifted will determine if the row is even or odd
	YRowShifted = (static_cast<int>(HexPositionX) % 2 == 0); // true if even, false if odd

	// Set XRowShifted based on whether the row is even or odd
	if (YRowShifted)  // If even row
	{
		XRowShifted = XRow;  // No shift
	}
	else  // If odd row
	{
		XRowShifted = XRow + (HexRadius * 0.75f);  // Shift odd rows to the right
	}
}


void AGridManager::OnTileClicked(const FIntVector HexPosition)
{
	UE_LOG(LogTemp, Warning, TEXT("Hex clicked at: %s"), *HexPosition.ToString());
}


// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();

	GenerateMap();
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

