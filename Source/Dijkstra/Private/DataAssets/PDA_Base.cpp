// Salvatore Spoto 2020


#include "DataAssets/PDA_Base.h"

FPrimaryAssetId UPDA_Base::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Base", GetFName());
}
