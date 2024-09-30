// M4thi4sL  (2024)

#pragma once

#include "CoreMinimal.h"
#include "EHexType.generated.h"

/**
 * Defines the pathing state of the hex. Does this hex allow walking or does it block?
 */
UENUM(BlueprintType) 
enum class EHexType  : uint8
{
	Walkable UMETA(DisplayName = "Walk"),
	Blockable UMETA(DisplayName = "Block")
};