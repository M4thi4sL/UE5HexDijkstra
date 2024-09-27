// M4thi4sL  (2024)


#include "GridManager.h"

#include "AsyncTreeDifferences.h"
#include "StartPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Hexagon.h"
#include "Engine/AssetManager.h"
#include "Algo/RandomShuffle.h"

// Sets default values
AGridManager::AGridManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

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
        for (int Row = 0; Row < Height; ++Row)
        {
            const FIntVector CubeCoord = OddqToCubeCoordinates(Row, Column);
            const float HexRadius = 100 + Distance;
            float XRow, YRow, XRowShifted;
            bool YRowShifted;

            // Calculate hex positions
            CalculateHexPositions(Column, Row, HexRadius, XRow, YRow, XRowShifted, YRowShifted);

            FVector Location = YRowShifted ? FVector(XRow, YRow, 0.0f) : FVector(XRow, XRowShifted, 0.0f);

            // Setup spawn parameters
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            SpawnParams.bDeferConstruction = true;  // Defer the actor's construction

            // Spawn actor but defer construction
            AHexagon* Hex = GetWorld()->SpawnActorDeferred<AHexagon>(HexagonToSpawn, FTransform(FRotator(0, 0, 0), Location), this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
            if (Hex)
            {
                Hex->HexPosition = CubeCoord;

                // Get a random hex asset
                int32 RandomIndex = FMath::RandRange(0, HexPool.Num() - 1);
                TSoftObjectPtr<UPDA_Hex> RandomHexPtr = HexPool[RandomIndex];

                // Load the asset synchronously (blocking)
                UPDA_Hex* LoadedHexAsset = RandomHexPtr.LoadSynchronous();
                if (!LoadedHexAsset)
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to load HexDataAsset for Hex at %s"), *CubeCoord.ToString());
                    continue; // Skip if loading failed
                }

                // Assign the loaded asset to HexDataAsset
                Hex->HexDataAsset = LoadedHexAsset;

                // Finish the spawning process
                UGameplayStatics::FinishSpawningActor(Hex, FTransform(FRotator(0, 0, 0), Location));
                Hex->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

                // Add to HexCells after everything is done
                HexCells.Add(CubeCoord, Hex);

                // Bind the click event
                Hex->HexClicked.AddDynamic(this, &AGridManager::OnTileClicked);

                // Debug log
                UE_LOG(LogTemp, Log, TEXT("Hex added to HexCells at %s, Hex Label: %s"), *CubeCoord.ToString(), *Hex->GetActorLabel());
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("HexCells Count: %d"), HexCells.Num());
}


void AGridManager::SetGoal()
{
	// Step 1: Get all the keys from HexCells
	TArray<FIntVector> AllHexKeys;
	HexCells.GetKeys(AllHexKeys);

	// Step 2: Shuffle the keys for randomness
	Algo::RandomShuffle(AllHexKeys);

	// Step 3: Iterate through the shuffled list to find a non-obstacle tile
	for (const FIntVector& HexCoord : AllHexKeys)
	{
		// Check if the tile is a non-obstacle
		if (!IsObstacle(HexCoord))
		{
			// Save the valid tile as the goal
			Goal = HexCoord;

			// Find the corresponding Hexagon in HexCells
			AHexagon** HexagonPtr = HexCells.Find(Goal);
			if (HexagonPtr && *HexagonPtr)
			{
				AHexagon* Hexagon = *HexagonPtr;

				// Get the world location of the hexagon actor
				const FVector SpawnLocation(0,0,0);
				const FRotator SpawnRotation = FRotator::ZeroRotator;
				const FActorSpawnParameters SpawnParams;

				// Spawn the actor at the hexagon's location
				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(StartPoint, SpawnLocation, SpawnRotation, SpawnParams);
				if (SpawnedActor) SpawnedActor->AttachToActor(Hexagon, FAttachmentTransformRules::KeepRelativeTransform);
				return;
			}
		}
	}

	// If no valid non-obstacle tile was found, log an error
	UE_LOG(LogTemp, Warning, TEXT("No non-obstacle hexagons found in HexCells!"));
}


FIntVector AGridManager::OddqToCubeCoordinates(int OddqRow, int OddqColumn)
{
	int z = OddqRow - (OddqColumn - (OddqColumn & 1)) / 2;
	int x  = OddqColumn;
	int y = -OddqColumn - z;
	
	return FIntVector(x,y,z);
}

void AGridManager::CalculateHexPositions(float HexPositionX, float HexPositionY, float HexRadius, 
										 float& XRow, float& YRow, float& XRowShifted, bool& YRowShifted)
{
	// The square root of 3, used for height calculations in hex grids
	const float sqrt3 = FMath::Sqrt(3.0f);

	// Calculate X position for the row
	XRow = (HexRadius * (3.0f / 2.0f)) * HexPositionX;

	// Calculate Y position (taking height into account and row shifting for odd columns)
	YRow = HexRadius * sqrt3 * HexPositionY;

	// Check if the current row should be shifted (odd row check)
	YRowShifted = static_cast<int>(HexPositionX) % 2 == 1;

	// Adjust the XRowShifted depending on whether it's an odd row or not
	XRowShifted = YRow + ( HexRadius * sqrt3 * 0.5f); // Shift X row by half a hex width if YRow is shifted
}

bool AGridManager::IsObstacle(const FIntVector Position)
{
	// Find the Hexagon at the given Position
	AHexagon** Hex = HexCells.Find(Position);
    
	if (Hex && *Hex) // Check if Hex exists and is valid
	{
		// Check if Hex has a valid HexDataAsset
		if (!(*Hex)->HexDataAsset)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hex at %s has no HexDataAsset."), *Position.ToString());
			return false;  // Early return if no data asset
		}

		// Get the HexType from the HexDataAsset
		EHexType HexType = (*Hex)->HexDataAsset->HexType;
		UE_LOG(LogTemp, Warning, TEXT("Hex at %s has HexType: %d"), *Position.ToString(), (int)HexType);

		// Return whether the HexType is Blockable
		return HexType == EHexType::Blockable;
	}

	// Return false if the Hex was not found or invalid
	return false;
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
	
