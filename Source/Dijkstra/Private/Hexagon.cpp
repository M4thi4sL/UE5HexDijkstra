// M4thi4sL  (2024)


#include "Hexagon.h"
#include "Async/Async.h"
#include "Engine/AssetManager.h"


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
		TArray<FSoftObjectPath> SoftObjectPaths;

		// Convert the TSoftObjectPtr references to FSoftObjectPath
		for (const TSoftObjectPtr<UObject>& SoftRef : SoftReferences)
		{
			if (SoftRef.IsValid())
			{
				SoftObjectPaths.Add(SoftRef.ToSoftObjectPath());
			}
		}

		// Now request the async load for all the soft object paths
		if (SoftObjectPaths.Num() > 0)
		{
			UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObjectPaths,FStreamableDelegate::CreateLambda([this]()\
				{
					// This code will run once all assets are loaded
					UE_LOG(LogTemp, Log, TEXT("All assets have been loaded"));

					// Set static mesh and material (assuming they're now available)
					if (HexDataAsset->Mesh.IsValid())StaticMeshComponent->SetStaticMesh(HexDataAsset->Mesh.Get());
					if (HexDataAsset->BaseMaterial.IsValid())StaticMeshComponent->SetMaterial(0, HexDataAsset->BaseMaterial.Get());
				})
			);
		}
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
	// Check if the NewHexState is different from our CurrentState
	if (CurrentState == NewHexState)
	{
		return;
	}
	
	// Save the previous state and update the CurrentState
	EHexState PreviousHexState = CurrentState;
	CurrentState = NewHexState;

	switch (NewHexState)
	{
		case EHexState::Normal:
			UAssetManager::GetStreamableManager().RequestAsyncLoad(
				HexDataAsset->BaseMaterial.ToSoftObjectPath(), 
				FStreamableDelegate::CreateLambda([this]()
				{
					// Ensure asset is loaded before setting material
					if (HexDataAsset->BaseMaterial.IsValid()) SetNewMaterial(HexDataAsset->BaseMaterial.Get());
				})
			);
			break;	

		case EHexState::Hovered:
			UAssetManager::GetStreamableManager().RequestAsyncLoad(
				HexDataAsset->HoverMaterial.ToSoftObjectPath(), 
				FStreamableDelegate::CreateLambda([this]()
				{
					// Ensure asset is loaded before setting material
					if (HexDataAsset->HoverMaterial.IsValid())SetNewMaterial(HexDataAsset->HoverMaterial.Get());
				})
			);
			break;	

		case EHexState::Selected:
			UAssetManager::GetStreamableManager().RequestAsyncLoad(
				HexDataAsset->SelectionMaterial.ToSoftObjectPath(), 
				FStreamableDelegate::CreateLambda([this]()
				{
					// Ensure asset is loaded before setting material
					if (HexDataAsset->SelectionMaterial.IsValid())SetNewMaterial(HexDataAsset->SelectionMaterial.Get());
				})
			);
			break;	

		case EHexState::Special:
			UAssetManager::GetStreamableManager().RequestAsyncLoad(
				HexDataAsset->HoverMaterial.ToSoftObjectPath(), 
				FStreamableDelegate::CreateLambda([this]()
				{
					// Ensure asset is loaded before setting material
					if (HexDataAsset->HoverMaterial.IsValid()) SetNewMaterial(HexDataAsset->HoverMaterial.Get());
				})
			);
			break;	

		default:
			break;
	}
}
