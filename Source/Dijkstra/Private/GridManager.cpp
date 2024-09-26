// M4thi4sL  (2024)


#include "GridManager.h"

#include "AsyncTreeDifferences.h"
#include "StartPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Hexagon.h"
#include "Engine/AssetManager.h"

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

			// Call the CalculateHexPositions function
			CalculateHexPositions(Column,Row , HexRadius, XRow, YRow, XRowShifted, YRowShifted);

			FVector Location;
			if (YRowShifted) 
				Location = FVector(XRow, YRow, 0.0f);
			else 
				Location = FVector(XRow,XRowShifted  , 0.0f); 

			// Setup spawn parameters
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.bDeferConstruction = true;  // Defer the actor's construction

			// Spawn actor but defer construction to allow setting variables
			AHexagon* Hex = GetWorld()->SpawnActorDeferred<AHexagon>(HexagonToSpawn, FTransform(FRotator(0, 0, 0), Location), this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			if (Hex)
			{
				// Set variables on the actor before finalizing the spawn
				Hex->HexPosition = (CubeCoord);
				int32 RandomIndex = FMath::RandRange(0, HexPool.Num() - 1);
				TSoftObjectPtr<UPDA_Hex> RandomHexPtr = HexPool[RandomIndex];

				UAssetManager::GetStreamableManager().RequestAsyncLoad(
					RandomHexPtr.ToSoftObjectPath(),
                    FStreamableDelegate::CreateLambda([this, Hex, RandomHexPtr,CubeCoord, Location]() 
					{
						// Ensure asset is loaded before assigning to HexDataAsset
						if (RandomHexPtr.IsValid()) Hex->HexDataAsset = RandomHexPtr.Get();

						// Finish the spawning process
						UGameplayStatics::FinishSpawningActor(Hex, FTransform(FRotator(0, 0, 0), Location));
						Hex->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
						Hex->HexClicked.AddDynamic(this, &AGridManager::OnTileClicked);
                    	HexCells.Add(CubeCoord, Hex);
					})
				);
			}
		}
	}
}


void AGridManager::SetGoal()
{
	
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
	
