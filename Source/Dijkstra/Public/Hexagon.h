// M4thi4sL  (2024)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PDA_Hex.h"
#include "EHexState.h"
#include "Hexagon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHexClickedDelegate, FIntVector, HexPosition);

UCLASS(Blueprintable)
class DIJKSTRA_API AHexagon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHexagon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	void SetNewMaterial(UMaterialInstance* NewMaterial);

	//Setup the hex tile, read all the required info from the Data-asset and respond accordingly
	UFUNCTION(BlueprintCallable, Category= "Hexagon")
	void SetupHex();

	// Function to call when the hexagon is clicked
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	void OnHexClicked(FIntVector CurrentHexPosition);
public:
	// Static Mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	// Event handling functions
	UFUNCTION()
	void OnMeshClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

	UFUNCTION()
	void OnMeshBeginCursorOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnMeshEndCursorOver(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void OnConstruction(const FTransform& Transform) override;
	
	//DataAsset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon", meta = (ExposeOnSpawn = true))
	UPDA_Hex* HexDataAsset;
	
	// HexState
	UPROPERTY(BlueprintReadWrite, Category = "Hexagon")
	EHexState CurrentState = EHexState::Normal;

	// Event dispatcher for hexagon clicks
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Hexagon")
	FHexClickedDelegate HexClicked;

	// cubic coordinate of the hex tile in the grid
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hexagon", meta = (ExposeOnSpawn = true))
	FIntVector HexPosition;

	// Update the current HexState value
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	void SetHexState(const EHexState NewHexState);
};
