// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "ShooterAI.h"
#include"BehaviorTree/BlackboardComponent.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update PlayerLocation If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AShooterAI* OwnerComponent = Cast<AShooterAI>(OwnerComp.GetAIOwner());

	AShooterSamCharacter* Player = OwnerComponent->PlayerCharacter;
	UBlackboardComponent* Blackboard = OwnerComponent->GetBlackboardComponent();
	if (OwnerComponent && Player && Blackboard)
	{
		if (OwnerComponent->LineOfSightTo(Player))
		{
			Blackboard->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());
			OwnerComponent->SetFocus(Player);
		}
		else
		{
			Blackboard->ClearValue(GetSelectedBlackboardKey());
			OwnerComponent->ClearFocus(EAIFocusPriority::Gameplay);
		}
	}
}
