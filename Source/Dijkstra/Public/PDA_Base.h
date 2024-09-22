// M4thi4sL  (2024)

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PDA_Base.generated.h"

/**
 * Example of a Primary Data Asset that holds soft references and loads them.
 */
UCLASS(Abstract)  // Exposes class to Blueprints and Editor
class DIJKSTRA_API UPDA_Base : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	virtual TArray<TSoftObjectPtr<UObject>> GetSoftReferences();
};