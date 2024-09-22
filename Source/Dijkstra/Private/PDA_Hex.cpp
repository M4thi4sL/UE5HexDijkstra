// M4thi4sL  (2024)


#include "PDA_Hex.h"

TArray<TSoftObjectPtr<UObject>> UPDA_Hex::GetSoftReferences()
{
	TArray<TSoftObjectPtr<UObject>> SoftReferences;

	// Add each soft reference to the array
	if (Mesh.IsValid()) SoftReferences.Add(Mesh);
	if (BaseMaterial.IsValid()) SoftReferences.Add(BaseMaterial);
	if (SelectionMaterial.IsValid()) SoftReferences.Add(SelectionMaterial);
	if (HoverMaterial.IsValid()) SoftReferences.Add(HoverMaterial);
	
	return SoftReferences;
}