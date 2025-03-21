// M4thi4sL  (2024)

#pragma once

#include "CoreMinimal.h"
#include "PDA_Base.h"
#include "Hexagon/EHexType.h"
#include "PDA_Hex.generated.h"

/**
 * Data-asset holding all hex related information, Mesh, materials, cost, ...
 */
UCLASS(BlueprintType)
class DIJKSTRA_API UPDA_Hex : public UPDA_Base
{
	GENERATED_BODY()
	
public:

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;;
	
	// Soft references to various objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (AssetBundles = "World"))
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (AssetBundles = "World"))
	TSoftObjectPtr<UMaterialInstance> BaseMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (AssetBundles = "World"))
	TSoftObjectPtr<UMaterialInstance> SelectionMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (AssetBundles = "World"))
	TSoftObjectPtr<UMaterialInstance> HoverMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	EHexType HexType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Setup")
	int TravelCost = 1;
	
};
