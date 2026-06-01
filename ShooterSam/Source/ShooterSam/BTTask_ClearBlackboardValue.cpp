// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBlackboardValue.h"
#include"BehaviorTree/BlackboardComponent.h"

UBTTask_ClearBlackboardValue::UBTTask_ClearBlackboardValue()
{
	NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type UBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Get the blackboard comp
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	// Clear the value of the selected key
	if (BlackboardComp)
	{
		BlackboardComp->ClearValue(GetSelectedBlackboardKey());
	}
	
	// Return Success or failed

	return EBTNodeResult::Succeeded;
}
