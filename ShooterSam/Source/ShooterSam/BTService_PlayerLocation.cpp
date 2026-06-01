// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"
#include "ShooterAI.h"
#include "BehaviorTree/BlackboardComponent.h"

#include"Kismet/GameplayStatics.h"

UBTService_PlayerLocation::UBTService_PlayerLocation()
{
	NodeName = TEXT("Get PlayerLocation");
}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AShooterAI* OwnerComponent = Cast<AShooterAI>(OwnerComp.GetAIOwner());

	AShooterSamCharacter* Player = OwnerComponent->PlayerCharacter;
	UBlackboardComponent* BlackboardComp = OwnerComponent->GetBlackboardComponent();

	if (OwnerComponent && Player && BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());
	}

	/*
		좋은 방법이 아니다.
		BT의 설계는 Blackboard에서 가져다 쓰는 것을 원칙으로 해야한다.
		
		// 간단한 location만 가져오면 됨으로 이 방법을 사용
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		if (PlayerPawn && BlackboardComp)
		{
			// KnownLocation을 Key로 한다 PL이 아님
			BlackboardComp->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
		}
	*/


	/*
		삽질 그냥 Key를 LastKnownPlayerLocation해서 등록하는것이 맞음
	if (OwnerComponent && BlackboardComp)
	{
		if (BlackboardComp->IsVectorValueSet(GetSelectedBlackboardKey()))
		{
			FVector PlayerLocation = BlackboardComp->GetValueAsVector(GetSelectedBlackboardKey());
			BlackboardComp->SetValueAsVector("LastKnownPlayerLocation", PlayerLocation);
		}
	}*/
}
