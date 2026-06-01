// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Shoot.h"
#include	"ShooterAI.h"

UBTTaskNode_Shoot::UBTTaskNode_Shoot()
{
	NodeName = TEXT("Shoot At Player");
}

EBTNodeResult::Type UBTTaskNode_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	AShooterAI* OwnerComponent = Cast<AShooterAI>(OwnerComp.GetAIOwner());
	if (OwnerComponent)
	{
		AShooterSamCharacter* OwnerCharacter = OwnerComponent->MyCharacter;
		AShooterSamCharacter* PlayerCharacter = OwnerComponent->PlayerCharacter;

		if (OwnerCharacter && PlayerCharacter && PlayerCharacter->IsAlive)
		{
			OwnerCharacter->Shoot();
			Result = EBTNodeResult::Succeeded;
		}
	}
	return Result;
}
