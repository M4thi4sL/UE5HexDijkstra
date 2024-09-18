// Salvatore Spoto 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hexagon.generated.h"

UCLASS()
class MYPROJECT3_API AHexagon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexagon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
