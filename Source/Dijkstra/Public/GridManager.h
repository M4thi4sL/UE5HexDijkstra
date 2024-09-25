// M4thi4sL  (2024)

#pragma once

#include "CoreMinimal.h"
#include "Hexagon.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"

UCLASS()
class DIJKSTRA_API AGridManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridManager();
	
	UFUNCTION(BlueprintCallable, Category="Grid|Setup")
	void GenerateMap();

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
	FIntVector OddqToCubeCoordinates(int OddqRow, int OddqColumn);

	//Calculate the tile coords based on its position
	UFUNCTION(BlueprintPure, Category="Hexgrid|Calculation")
	void CalculateHexPositions(UPARAM() float HexPositionX, UPARAM() float HexPositionY, UPARAM() float HexRadius, 
							   float& XRow, float& YRow, float& XRowShifted, bool& YRowShifted);

	UFUNCTION(Category="Grid")
	void OnTileClicked(const FIntVector HexPosition);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// The width of the hex grid
	UPROPERTY(BlueprintReadWrite, Category="Grid|Setup")
	int Width = 8;

	//The height of the hex grid
	UPROPERTY(BlueprintReadWrite, Category="Grid|Setup")
	int Height = 8;

	//The spacing in between two different Hexes.
	//We already account for the individual size of the Hex so this is purely for the space between two hexes.
	UPROPERTY(BlueprintReadWrite, Category="Grid|Setup")
	float Distance = 2;

	//Tile we want to spawn.
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AHexagon> HexagonToSpawn;

private:
	//Map containing all the Hexes in the grid
	UPROPERTY()
	TMap<FIntVector, AHexagon*> HexCells;

	//FVector Goal, in Cubic coordinate. 
	UPROPERTY()
	FIntVector Goal = FIntVector(0,0,0);
	
	

};
