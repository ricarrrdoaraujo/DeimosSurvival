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
	UPROPERTY(EditDefaultsOnly)
	class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTreeComponent* BehaviorTreeComp;

	UPROPERTY(EditDefaultsOnly)
	class UBlackboardComponent* BlackBoardComp;
	
	//Função delegate que será chamada
	//quando a função OnSeePawn ocorrer
	UFUNCTION()
	void OnSeePawn(class APawn* SensePawn);
};
