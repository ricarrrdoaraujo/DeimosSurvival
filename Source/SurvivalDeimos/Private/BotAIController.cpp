// Fill out your copyright notice in the Description page of Project Settings.


#include "BotAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "BotCharacter.h"

ABotAIController::ABotAIController()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(FName(TEXT("PawnSensingComp")));
	PawnSensingComp->SensingInterval = .25f;
	PawnSensingComp->bOnlySensePlayers = true;
	PawnSensingComp->SetPeripheralVisionAngle(180.f);
	PawnSensingComp->SightRadius = 3000.f;

	BlackBoardComp = CreateDefaultSubobject<UBlackboardComponent>(FName("BackBoardComp"));
}

void ABotAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PawnSensingComp->OnSeePawn.AddDynamic(this,
		&ABotAIController::OnSeePawn);

	if (BehaviorTree)
	{
		BlackBoardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorTreeComp->StartTree(*BehaviorTree);
		BlackBoardComp->SetValueAsBool("DevePerambular", true);
	}
}

void ABotAIController::OnSeePawn(APawn* SensePawn)
{
}
