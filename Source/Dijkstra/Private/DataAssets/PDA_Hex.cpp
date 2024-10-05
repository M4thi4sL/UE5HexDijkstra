// M4thi4sL  (2024)


#include "DataAssets/PDA_Hex.h"

TArray<TSoftObjectPtr<UObject>> UPDA_Hex::GetSoftReferences()
{
	TArray<TSoftObjectPtr<UObject>> SoftReferences;

	// Add each soft reference to the array only if it is NOT null (i.e., assigned)
	if (!Mesh.IsNull()) SoftReferences.Add(Mesh);
	if (!BaseMaterial.IsNull()) SoftReferences.Add(BaseMaterial);
	if (!SelectionMaterial.IsNull()) SoftReferences.Add(SelectionMaterial);
	if (!HoverMaterial.IsNull()) SoftReferences.Add(HoverMaterial);
    
	return SoftReferences;
}