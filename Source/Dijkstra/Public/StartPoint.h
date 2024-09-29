// M4thi4sL  (2024)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StartPoint.generated.h"

UCLASS()
class DIJKSTRA_API AStartPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStartPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
