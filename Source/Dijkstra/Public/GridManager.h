// M4thi4sL  (2024)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hexagon.h"
#include "PDA_Hex.h"
#include "StartPoint.h"
#include "GridManager.generated.h"

UCLASS()
class DIJKSTRA_API AGridManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridManager();
	
	UFUNCTION(BlueprintCallable, Category="Grid|Setup", CallInEditor)
	void GenerateMap();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category="Grid")
	void OnTileClicked(const FIntVector HexPosition);
	
private:

	//Grid generation

	//Clear an existing grid.
	UFUNCTION(BlueprintCallable, Category="Grid|Setup")
	void ClearGrid();

	//Generate a new grid.
	UFUNCTION(BlueprintCallable, Category="Grid|Setup")
	void GenerateGrid();

	//Set a hex in the grid as the goal.
	UFUNCTION(BlueprintCallable, Category="Grid|Setup")
	void SetGoal();

	//Hex specific functions

	//Convert Oddq to Cuve Coords.
	UFUNCTION(BlueprintPure, Category="Grid|Conversion")
	static FIntVector OddqToCubeCoordinates(int OddqRow, int OddqColumn);

	//Calculate the tile coords based on its position
	UFUNCTION(BlueprintPure, Category="Hexgrid|Calculation")
	static void CalculateHexPositions(UPARAM() const int Column, UPARAM() const int Row, UPARAM()const float HexRadius, 
							   float& XRow, float& YRow, float& XRowShifted, bool& YRowShifted);

	UFUNCTION(BlueprintCallable, Category="HexGrid|Dijkstra")
	TMap<FIntVector, FIntVector> FindPath(UPARAM()FIntVector StartPosition, bool& PathFound);

	UFUNCTION(BlueprintCallable, Category="HexGrid|Dijkstra")
	void DrawPath(UPARAM()const FIntVector StartPosition,UPARAM() const TMap<FIntVector, FIntVector>PathToTake);
	
	UFUNCTION(BlueprintPure, Category="HexGrid|Dijkstra")
	FIntVector GetLowestCostHexCoordinates(const TArray<FIntVector>& Neighbours, const TMap<FIntVector, int32>& CostSoFar, const TArray<FIntVector>& Visited);

	UFUNCTION(BlueprintCallable, Category="Hexgrid|Dijkstra")
	TArray<FIntVector>GetValidUnvisitedNeighbours(const FIntVector &Current, const TArray<FIntVector>&VisitedHex);
	
	//Is the tile at the given position considered an obstacle?
	UFUNCTION(BlueprintPure, Category="Grid|Helpers")
	bool IsObstacle(UPARAM() const FIntVector Position);

	//Set all tiles in the tilemap to a certain state
	UFUNCTION(BlueprintCallable, Category="Grid")
	void SetAllHexState(UPARAM()const EHexState NewHexState);

	// Highlight the Neighbouring Hex Tiles of the Selected Hex;
	UFUNCTION(BlueprintCallable)
	void SetNeighbourHexHighlighted(UPARAM()const FIntVector& SelectedHex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Add a scene component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootSceneComponent;
	
	// The width of the hex grid
	UPROPERTY(EditAnywhere, Category="Grid|Setup")
	int Width = 8;

	//The height of the hex grid
	UPROPERTY(EditAnywhere, Category="Grid|Setup")
	int Height = 8;

	//The spacing in between two different Hexes.
	//We already account for the individual size of the Hex so this is purely for the space between two hexes.
	UPROPERTY(EditAnywhere, Category="Grid|Setup")
	float Distance = 2;

	//Tile we want to spawn.
	UPROPERTY(EditAnywhere, Category = "Grid|Setup")
	TSubclassOf<AHexagon> HexagonToSpawn;

	//Start point class
	UPROPERTY(EditAnywhere, Category = "Grid|Setup")
	TSubclassOf<AStartPoint> StartPoint;

	//Map containing all the coordinates and tiles in the grid.
	UPROPERTY(EditAnywhere, Category = "Grid|Setup")
	TArray<TSoftObjectPtr<UPDA_Hex>> HexPool;

	//Map containing all the Hexes in the grid
	UPROPERTY(BlueprintReadOnly)
	TMap<FIntVector, AHexagon*> HexCells;

	//FVector Goal, in Cubic coordinate. 
	UPROPERTY(BlueprintReadOnly)
	FIntVector Goal = FIntVector(0,0,0);

};
