// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetOwner()->GetActorLocation();
	SetShouldMove(false);	// 처음 목적지를 설정하려면 필요

}

bool UMover::GetShouldMove()
{
	return ShouldMove;
}

void UMover::SetShouldMove(bool NewShouldMove)
{
	ShouldMove = NewShouldMove;

	if (true == ShouldMove)
	{
		// Target location is start location + move offset
		TargetLoaction = StartLocation + MoveOffset;
	}
	else
	{
		// Target location is start location
		TargetLoaction = StartLocation;
	}
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	{
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		FVector NewLocation;

		ReachedTarget = CurrentLocation.Equals(TargetLoaction);
		if (!ReachedTarget)
		{
			float Speed = MoveOffset.Length() / MoveTime;
			NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLoaction, DeltaTime, Speed);
			GetOwner()->SetActorLocation(NewLocation);

		}
	}

}

