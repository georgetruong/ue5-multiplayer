// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBox.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMyBox::AMyBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//bReplicates = true;
	ReplicatedVar = 100.0f;
}

// Called when the game starts or when spawned
void AMyBox::BeginPlay()
{
	Super::BeginPlay();

	SetReplicates(true);
	SetReplicateMovement(true);

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &AMyBox::DecreaseReplicatedVar, 2.0f, false);
	}
}

// Called every frame
void AMyBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if 0
	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Server"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Client"));
	}
#endif

}

void AMyBox::OnRep_ReplicatedVar()
{
	if (HasAuthority())
	{
		FVector NewLocation = GetActorLocation() + FVector(0.0f, 0.0f, 200.0f);
		SetActorLocation(NewLocation);

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Server: OnRep_ReplicatedVar"));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, 
		//	FString::Printf(TEXT("Client %d: OnRep_ReplicatedVar"), GPlayInEditorID));
	}
}

void AMyBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyBox, ReplicatedVar);
}

void AMyBox::DecreaseReplicatedVar()
{
	if (HasAuthority())
	{
		ReplicatedVar -= 1.0f;
		OnRep_ReplicatedVar();

		if (ReplicatedVar > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &AMyBox::DecreaseReplicatedVar, 2.0f, false);
		}
	}
}

