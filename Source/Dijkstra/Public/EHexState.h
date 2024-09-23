// M4thi4sL  (2024)

#pragma once

#include "CoreMinimal.h"
#include "EHexState.generated.h"

/**
 * Defines the state of the hex
 */
UENUM(BlueprintType) 
enum class EHexState  : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Hovered UMETA(DisplayName = "Hovered"),
	Selected UMETA(Displayname = "Selected")
};