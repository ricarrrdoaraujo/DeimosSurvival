// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BotAIController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALDEIMOS_API ABotAIController : public AAIController
{
	GENERATED_BODY()

protected:
	ABotAIController();

	//Função responsável por dar o controle do Pawn(inimigo)
	//ao Controller. Ou seja, BotChararter será controlado pelo
	//ABotAiController quando for possuído por este Controlador
	virtual void OnPossess(APawn* InPawn) override;

	//O SensingComponent encapsula configuracoes e funcionalidades
	//sensoriais(vião, audição) de um ator.
	UPROPERTY(EdityDefaultsOnly)
	class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EdityDefaultsOnly)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EdityDefaultsOnly)
	class UBehaviorTreeComponent* BehaviorTreeComp;

	UPROPERTY(EdityDefaultsOnly)
	class UBlackBoardComponent* BlackBoardComp;
	
	UFUNCTION()
	void OnSeePawn(class APawn* SensePawn);
};
