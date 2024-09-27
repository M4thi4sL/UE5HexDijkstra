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
	
	UFUNCTION(BlueprintCallable, Category="Grid | Setup", CallInEditor)
	void GenerateMap();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:

	//Grid generation

	//Clear an existing grid.
	UFUNCTION(BlueprintCallable, Category="Grid | Setup")
	void ClearGrid();

	//Generate a new grid.
	UFUNCTION(BlueprintCallable, Category="Grid | Setup")
	void GenerateGrid();

	//Set a hex in the grid as the goal.
	UFUNCTION(BlueprintCallable, Category="Grid | Setup")
	void SetGoal();

	//Hex specific functions

	//Convert Oddq to Cuve Coords.
	UFUNCTION(BlueprintPure, Category="Grid | Conversion")
	static FIntVector OddqToCubeCoordinates(int OddqRow, int OddqColumn);

	//Calculate the tile coords based on its position
	UFUNCTION(BlueprintPure, Category="Hexgrid | Calculation")
	static void CalculateHexPositions(UPARAM() float HexPositionX, UPARAM() float HexPositionY, UPARAM() float HexRadius, 
							   float& XRow, float& YRow, float& XRowShifted, bool& YRowShifted);


	//Is the tile at the given position considered an obstacle?
	UFUNCTION(Category="Grid | Helpers")
	bool IsObstacle(UPARAM() const FIntVector Position);
	
	UFUNCTION(Category="Grid")
	void OnTileClicked(const FIntVector HexPosition);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Add a scene component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootSceneComponent;
	
	// The width of the hex grid
	UPROPERTY(EditAnywhere, Category="Grid | Setup")
	int Width = 8;

	//The height of the hex grid
	UPROPERTY(EditAnywhere, Category="Grid | Setup")
	int Height = 8;

	//The spacing in between two different Hexes.
	//We already account for the individual size of the Hex so this is purely for the space between two hexes.
	UPROPERTY(EditAnywhere, Category="Grid | Setup")
	float Distance = 2;

	//Tile we want to spawn.
	UPROPERTY(EditAnywhere, Category = "Grid | Setup")
	TSubclassOf<AHexagon> HexagonToSpawn;

	//Start point class
	UPROPERTY(EditAnywhere, Category = "Grid | Setup")
	TSubclassOf<AStartPoint> StartPoint;

	//Map containing all the coordinates and tiles in the grid.
	UPROPERTY(EditAnywhere, Category = "Grid | Setup")
	TArray<TSoftObjectPtr<UPDA_Hex>> HexPool;

private:
	//Map containing all the Hexes in the grid
	UPROPERTY()
	TMap<FIntVector, AHexagon*> HexCells;

	//FVector Goal, in Cubic coordinate. 
	UPROPERTY()
	FIntVector Goal = FIntVector(0,0,0);
	
	

};
