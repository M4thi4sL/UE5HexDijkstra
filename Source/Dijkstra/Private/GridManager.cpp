// M4thi4sL  (2024)


#include "GridManager.h"
#include "StartPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Hexagon/Hexagon.h"
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
	// Use a TSet to ensure unique actors
	TSet<AActor*> ActorsToClear;

	// Get all Hexagon actors and add them to the set
	TArray<AActor*> Hexagons;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHexagon::StaticClass(), Hexagons);
	ActorsToClear.Append(Hexagons);

	// Get all StartPoint actors and add them to the set
	TArray<AActor*> StartPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStartPoint::StaticClass(), StartPoints);
	ActorsToClear.Append(StartPoints);

	// Safely destroy all unique actors
	for (AActor* Actor : ActorsToClear)
	{
		Actor->Destroy();
	}

	// Clear the Hexcell Map
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

            FVector Location = YRowShifted ? FVector(XRow, XRowShifted, 0.0f) : FVector(XRow,YRow , 0.0f);

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
                int RandomIndex = FMath::RandRange(0, HexPool.Num() - 1);
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
            	#if WITH_EDITOR
                UE_LOG(LogTemp, Log, TEXT("Hex added to HexCells at %s, Hex Label: %s"), *CubeCoord.ToString(), *Hex->GetActorLabel());
				#endif
            	
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

				Hexagon->SetHexState(EHexState::Special);

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
	int x = OddqColumn;
	int z = OddqRow - (OddqColumn - (OddqColumn & 1)) / 2; // Calculate Z based on column offset
	int y = -x - z; // Ensure Q + R + S = 0
	return FIntVector(x, y, z);
}

void AGridManager::CalculateHexPositions(const int Column,const int Row, const float HexRadius, 
										 float& XRow, float& YRow, float& XRowShifted, bool& YRowShifted)
{
	const float sqrt3 = FMath::Sqrt(3.0f);
	YRow = HexRadius * sqrt3 * Row;
	XRow = (HexRadius * (3.0f / 2.0f)) * Column;
	// Check if the current row should be shifted (odd row check)
	YRowShifted = static_cast<int>(Column) % 2 == 1;

	// Adjust the XRowShifted depending on whether it's an odd row or not
	XRowShifted = YRow + ( HexRadius * sqrt3 * 0.5f); // Shift X row by half a hex width if YRow is shifted
}
TMap<FIntVector, FIntVector> AGridManager::FindPath(FIntVector StartPosition, bool& PathFound)
{
    // Initialize the targets array with the start node
    TArray<FIntVector> Targets;
    Targets.Add(StartPosition);

	// Clear the priority queue to start fresh
	PriorityQueue.Clear();
	PriorityQueue.Push(StartPosition, 0.0f);

    // Initialize the VisitedHex array
    TArray<FIntVector> VisitedHex;

    // Initialize the PathToTake Map, traversing this map in reverse order gives us the shortest path
    TMap<FIntVector, FIntVector> PathToTake;
    
    // Keep looping while there are targets to explore
    while (Targets.Num() > 0)
    {
        // Get the hex with the lowest cost from the targets
        float CurrentPriority;
    	FIntVector Current = PriorityQueue.Peek(EPeekPosition::Head, CurrentPriority);
    	PriorityQueue.Pop(); // Remove the element after peeking
        
        // If the current hex is the goal, break the loop
        if (Current == Goal)
        {
            break;
        }

        // Find the neighbour Hex tiles based on the current, exclude Hex tiles we have already visited
        const TArray<FIntVector> Neighbours = GetValidUnvisitedNeighbours(Current, VisitedHex);
        const AHexagon* CurrentHex = *HexCells.Find(Current);

        for (FIntVector Neighbour : Neighbours)
        {
            float OldCost;
            bool bNeighbourInQueue = PriorityQueue.Find(Neighbour, OldCost);
            
            // Calculate the new cost to reach the neighbour
            int NewCost = static_cast<int>(CurrentPriority) + CurrentHex->HexDataAsset->TravelCost;

            // Only update cost if this is a new neighbor or the new cost is lower
            if (!bNeighbourInQueue || NewCost < OldCost)
            {
                // Update the cost so far for the neighbor
                PriorityQueue.Push(Neighbour, NewCost);

                // Add the neighbor to the targets if not already added
                if (!Targets.Contains(Neighbour))
                {
                    Targets.Add(Neighbour);
                }

                // Record the path: Neighbour came from Current
                PathToTake.Add(Neighbour, Current);
            }
        }

        // Mark the current hex as visited
        VisitedHex.Add(Current);
    }

    // Check if the goal was found in the path
    PathFound = PathToTake.Contains(Goal);
    return PathToTake;
}


void AGridManager::DrawPath(const FIntVector StartPosition,const TMap<FIntVector, FIntVector> PathToTake)
{
	// Step 1: Reset all the hex tiles to normal state (excluding the goal)
	SetAllHexState(EHexState::Normal);

	// Step 2: Traverse the PathToTake from goal to start and build the path
	TArray<FIntVector> Path;
	FIntVector CurrentHexCoord = Goal;

	while (CurrentHexCoord != StartPosition)
	{
		// Add the current hex to the path array
		Path.Add(CurrentHexCoord);

		// Find the next hex in the path
		const FIntVector* NextHexCoord = PathToTake.Find(CurrentHexCoord);

		// If no valid next hex is found, log a warning and break the loop
		if (!NextHexCoord)
		{
			UE_LOG(LogTemp, Warning, TEXT("PathToTake is incomplete or broken at hex: %s"), *CurrentHexCoord.ToString());
			break;
		}

		// Move to the next hex in the path
		CurrentHexCoord = *NextHexCoord;
	}

	// Add the start position to the path (since we don't include it in the while loop)
	Path.Add(StartPosition);

	// Step 3: Reverse the path, so we can traverse it from start to goal
	Algo::Reverse(Path);

	// Step 4: Set each hex in the path to "Selected"
	for (const FIntVector& HexCoord : Path)
	{
		if (AHexagon** Hex = HexCells.Find(HexCoord); Hex && *Hex)
		{
			(*Hex)->SetHexState(EHexState::Selected);
		}
	}

	// Step 5: Set the start and goal hex tiles to "special" so they have a different color.
	if (AHexagon** StartHex = HexCells.Find(StartPosition); StartHex && *StartHex)
	{
		(*StartHex)->SetHexState(EHexState::Special);
	}

	if (AHexagon** GoalHex = HexCells.Find(Goal); GoalHex && *GoalHex)
	{
		(*GoalHex)->SetHexState(EHexState::Special);
	}
}

TArray<FIntVector> AGridManager::GetValidUnvisitedNeighbours(const FIntVector& Current,	const TArray<FIntVector>& VisitedHex)
{
	TArray<FIntVector>Neighbours;
	
	// Initialise the Cubic Coordinates directions
	static const TArray<FIntVector> CubeDirections = {
		FIntVector(1, -1, 0),   // Right
		FIntVector(1, 0, -1),   // Upper-right
		FIntVector(0, 1, -1),   // Upper-left
		FIntVector(-1, 1, 0),   // Left
		FIntVector(-1, 0, 1),   // Lower-left
		FIntVector(0, -1, 1)    // Lower-right
	};
	
	for (const FIntVector Direction : CubeDirections)
	{
		FIntVector NeighbourHex = Current + Direction;
		if (!VisitedHex.Contains(NeighbourHex))
		{
			if (HexCells.Contains(NeighbourHex) && !IsObstacle(NeighbourHex))
			{
				Neighbours.Add(NeighbourHex);
			}
		}
	}

	return Neighbours;
}

bool AGridManager::IsObstacle(const FIntVector Position)
{
	// Find the Hexagon at the given Position
	AHexagon** Hex = HexCells.Find(Position);
    
	if (Hex && *Hex) // Check if Hex exists and is valid
	{
		// Check if Hex has a valid HexDataAsset
		if (!(*Hex)->HexDataAsset) return false;

		// Get the HexType from the HexDataAsset
		EHexType HexType = (*Hex)->HexDataAsset->HexType;

		// Return whether the HexType is Blockable
		return HexType == EHexType::Blockable;
	}
	// Return false if the Hex was not found or invalid
	return false;
}

void AGridManager::SetAllHexState(const EHexState NewHexState)
{
	TArray<FIntVector> AllHexKeys;
	HexCells.GetKeys(AllHexKeys);
	AllHexKeys.Remove(Goal);

	// Iterate over all hex keys (positions)
	for (const FIntVector& Key : AllHexKeys)
	{
		// Check if the key exists in the HexCells map
		AHexagon** Hex = HexCells.Find(Key);
        
		// If a hexagon is found at the given key, set its state
		if (Hex && *Hex)
		{
			(*Hex)->SetHexState(NewHexState);
		}
	}

}

void AGridManager::SetNeighbourHexHighlighted(const FIntVector& SelectedHex)
{
	TArray<FIntVector> VisitedHex;
	TArray<FIntVector> ValidNeighbours = GetValidUnvisitedNeighbours(SelectedHex, VisitedHex);

	SetAllHexState(EHexState::Normal);
	// Loop over the result and process each valid neighbour
	for (const FIntVector& Neighbour : ValidNeighbours)
	{
		AHexagon** Hex = HexCells.Find(Neighbour);
		if (Hex && *Hex)
		{
			(*Hex)->SetHexState(EHexState::Selected);
		}
	}
	
	if (AHexagon** Selection = HexCells.Find(SelectedHex); Selection && *Selection)
	{
		(*Selection)->SetHexState(EHexState::Special);
	}
}

void AGridManager::OnTileClicked(const FIntVector HexPosition)
{
	UE_LOG(LogTemp, Log, TEXT("Hex clicked at: %s"), *HexPosition.ToString());

	//Debug draws the neighbouring hexes 
	// SetNeighbourHexHighlighted(HexPosition);

	// Find the path, then draw the path we found.
	bool PathFound;
	const TMap<FIntVector,FIntVector> PathToTake = FindPath(HexPosition, PathFound);
	if (PathFound)
	{
	DrawPath(HexPosition, PathToTake );
	}
	else
	{
		SetAllHexState(EHexState::Normal);
	}
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();

	PriorityQueue = TPriorityQueue<FIntVector>(EPriorityOrder::Ascending);

	GenerateMap();
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
	
