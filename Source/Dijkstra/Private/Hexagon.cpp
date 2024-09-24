// M4thi4sL  (2024)


#include "Hexagon.h"
#include "Async/Async.h"


// Sets default values
AHexagon::AHexagon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	StaticMeshComponent->OnClicked.AddDynamic(this, &AHexagon::OnMeshClicked);
	StaticMeshComponent->OnBeginCursorOver.AddDynamic(this, &AHexagon::OnMeshBeginCursorOver);
	StaticMeshComponent->OnEndCursorOver.AddDynamic(this, &AHexagon::OnMeshEndCursorOver);
}

void AHexagon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetupHex();
}


// Called when the game starts or when spawned
void AHexagon::BeginPlay()
{
	Super::BeginPlay();
	SetupHex();
}

void AHexagon::SetNewMaterial(UMaterialInstance* NewMaterial)
{
	if (StaticMeshComponent && NewMaterial)
	{
		// Set the new material to the Static Mesh component
		StaticMeshComponent->SetMaterial(0, NewMaterial); 
	}
}

void AHexagon::SetupHex()
{
	if (HexDataAsset)
	{
		// Get the array of soft references
		TArray<TSoftObjectPtr<UObject>> SoftReferences = HexDataAsset->GetSoftReferences();

		// Load each soft reference asynchronously
		for (const TSoftObjectPtr<UObject>& SoftRef : SoftReferences)
		{
			// Check if the reference is valid before trying to load
			if (SoftRef.IsValid())
			{
				// Load the asset synchronously
				UObject* LoadedAsset = SoftRef.LoadSynchronous();
			}
		}
		StaticMeshComponent->SetStaticMesh(HexDataAsset->Mesh.Get());
		StaticMeshComponent->SetMaterial(0, HexDataAsset->BaseMaterial.Get());
	}
}

void AHexagon::OnMeshClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	switch (HexDataAsset->HexType)
	{
	case EHexType::Walkable:
		HexClicked.Broadcast(HexPosition);
		break;
	default:
		break;
	}
	
}

void AHexagon::OnHexClicked(FIntVector CurrentHexPosition)
{
	HexClicked.Broadcast(HexPosition);
}

void AHexagon::OnMeshBeginCursorOver(UPrimitiveComponent* TouchedComponent)
{
	switch(CurrentState)
	{
		case  EHexState::Normal:
		switch(HexDataAsset->HexType)
		{
		case EHexType::Walkable:
			SetHexState(EHexState::Hovered);
			break;
		default:
			break;
		}
	default:
		break;
	}
}

void AHexagon::OnMeshEndCursorOver(UPrimitiveComponent* TouchedComponent)
{
	switch(CurrentState)
	{
	case  EHexState::Hovered:
		SetHexState(EHexState::Normal);
		break;
	default:
		break;
	}
}

void AHexagon::SetHexState(const EHexState NewHexState)
{
	//check if the NewHexState is different from our CurrentState as there is no point in updating to the same value.
	if (CurrentState == NewHexState)
	{
		return;
	}
	
	// save the previous state and Update the CurrentState to the new state
	EHexState PreviousHexState = CurrentState;
	CurrentState = NewHexState;
	UMaterialInstance* Material = nullptr;
	
	switch (NewHexState)
	{
	case EHexState::Normal:
		Material = HexDataAsset->BaseMaterial.LoadSynchronous();
		SetNewMaterial(Material);
		break;
	case EHexState::Hovered:
		Material = HexDataAsset->HoverMaterial.LoadSynchronous();
		SetNewMaterial(Material);
		break;
	case EHexState::Selected:
		Material = HexDataAsset->SelectionMaterial.LoadSynchronous();
		SetNewMaterial(Material);
		break;
	case EHexState::Special:
		Material = HexDataAsset->HoverMaterial.LoadSynchronous();
		SetNewMaterial(Material);
		break;
	default:
			break;
	}
}
