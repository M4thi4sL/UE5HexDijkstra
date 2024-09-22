// M4thi4sL  (2024)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PDA_Hex.h"
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

public:
	//DataAsset
	UPROPERTY(BlueprintReadWrite, Category= "Hexagon")
	UPDA_Hex* HexDataAsset;
	
	// Event dispatcher for hexagon clicks
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Hexagon")
	FHexClickedDelegate HexClicked;

	// Function to call when the hexagon is clicked
	UFUNCTION(BlueprintCallable, Category = "Hexagon")
	void OnHexClicked(FIntVector HexPosition);
};
