// M4thi4sL  (2024)

#pragma once

#include "CoreMinimal.h"
#include "PDA_Base.h"
#include "EHexType.h"
#include "PDA_Hex.generated.h"

/**
 * Data-asset holding all hex related information, Mesh, materials, cost, ...
 */
UCLASS(BlueprintType)
class DIJKSTRA_API UPDA_Hex : public UPDA_Base
{
	GENERATED_BODY()
	
public:
	// Soft references to various objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TSoftObjectPtr<UMaterialInstance> BaseMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TSoftObjectPtr<UMaterialInstance> SelectionMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TSoftObjectPtr<UMaterialInstance> HoverMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	EHexType HexType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Setup")
	int TravelCost = 1;
	
	// Function that returns an array of loaded objects (matches the blueprint function)
	virtual TArray<TSoftObjectPtr<UObject>> GetSoftReferences() override;
	
};
