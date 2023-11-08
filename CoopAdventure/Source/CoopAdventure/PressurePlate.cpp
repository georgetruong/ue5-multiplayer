// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"

// Sets default values
APressurePlate::APressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);
	
	Activated = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trigger Mesh"));
	TriggerMesh->SetupAttachment(RootComp);
	TriggerMesh->SetIsReplicated(true);

	auto TriggerMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
		TEXT("/Game/StarterContent/Shapes/Shape_Cylinder"));
	if (TriggerMeshAsset.Succeeded())
	{
		TriggerMesh->SetStaticMesh(TriggerMeshAsset.Object);
		TriggerMesh->SetRelativeScale3D(FVector(3.3f, 3.3f, 0.2f));
		TriggerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(
		TEXT("/Game/PolygonPrototype/Meshes/FX/SM_FX_Glow_Ring_01"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetRelativeScale3D(FVector(4.0f, 4.0f, 0.5f));
		Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 7.2f));
	}
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();

	TriggerMesh->SetVisibility(false);
	TriggerMesh->SetCollisionProfileName(FName("OverlapAll"));
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		AActor* TriggerActor = 0;
		TArray<AActor*> OverlappingActors;
		TriggerMesh->GetOverlappingActors(OverlappingActors);
		for (int ActorIdx = 0; ActorIdx < OverlappingActors.Num(); ++ActorIdx)
		{
			AActor* A = OverlappingActors[ActorIdx];
			if (A->ActorHasTag("TriggerActor"))
			{
				TriggerActor = A;
				break;
			}

			//FString Msg = FString::Printf(TEXT("Name: %s"), *A->GetName());
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, Msg);
		}

		if (TriggerActor)
		{
			if (!Activated)
			{
				Activated = true;
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("Activated"));
			}
		}
		else
		{
			if (Activated)
			{
				Activated = false;
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("Deactivated"));
			}
		}
	}

}

