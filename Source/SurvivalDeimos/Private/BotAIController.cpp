// Fill out your copyright notice in the Description page of Project Settings.


#include "BotAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "BotCharacter.h"
#include "Engine/Engine.h"
#include "Arma.h"
#include "PersonagemTPS.h"

ABotAIController::ABotAIController()
{
	//Contrutor que cria o component de sensores do BotInimigo
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(FName(TEXT("PawnSensingComp")));

	//**Como os sensores do inimigo dever�o atuar**
	//Deve fazer a varredura sensorial a cada .25 segundos
	PawnSensingComp->SensingInterval = .25f;
	//Deve tetectar, sersensivel a detec��o apenas de players
	PawnSensingComp->bOnlySensePlayers = true;
	//O �ngulo de vis�o deve ser de 180 graus, isso facilita ver de costas
	PawnSensingComp->SetPeripheralVisionAngle(180.f);
	//O raio de vis�o ser� de 3000 unidades
	PawnSensingComp->SightRadius = 3000.f;

	//Componente de BT para manipular a BT criada
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("BehaviorTreeComp"));
	//Da mesma forma criando um componente de blackboard para podermos colocar variaveis, alterar seu valor, etc
	BlackBoardComp = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackBoardComp"));
}

void ABotAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	////Se o sensor do inimigo disparar um evento,
	//significa que ele detectou o jogador
	//que para ele � seu inimigo.
	PawnSensingComp->OnSeePawn.AddDynamic(this,
		&ABotAIController::OnSeePawn);

	if (BehaviorTree)
	{
		//Iniciando Blackboard e BehaviorTree
		BlackBoardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorTreeComp->StartTree(*BehaviorTree);
		//Se o bot ainda n�o viu o jogador, ele deve ficar patrulhando
		BlackBoardComp->SetValueAsBool("DevePerambular", true);
	}
}

void ABotAIController::OnSeePawn(APawn* SensedPawn)
{
	ABotCharacter* Bot = Cast<ABotCharacter>(GetPawn());
	//OnSeePawn recebe o objeto detectado no sensor do inimigo(BotCharacter)
	//Mas SensedPawn retorna um objeto da classe gen�rica do player (APawn)
	//Fazendo um cast para a classe do Player e colocando o resultado
	//deste objeto especializafo em Player
	APersonagemTPS* Player = Cast<APersonagemTPS>(SensedPawn);
	//Somente se o inimigo tiver Health ele vai continuar a atirar
	// e perseguir o Player
	//Se o inimigo ou o player estiver morto, coloque enemy como nulo
	//para parar o Bot de perseguir ele, pois na BehaviorTree n�o 
	//estar� setado se for nullptr
	if (Bot->GetHealth() <= 0.0f || Player->IsEstaMorto())
	{
		BlackBoardComp->SetValueAsObject("Inimigo", nullptr);
		return;
	}

	//Se o Health do bot for maior que zero e o player n�o
	//estiver morto, continue a perseguir e atire no player
	if (Bot->GetHealth() > 0.0f && !Player->IsEstaMorto())
	{
		if (BlackBoardComp && SensedPawn)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Eu estou te vendo"));
			//Setando a vari�vel do Blackboard Inimigo com o valor do objeto.
			//SensedPawn: Este objeto � retornado quando o game est� em execu��o
			//e o sensor do inimigo detecta seu inimigo(no caso o player)
			//Ent�o o objeto player � colocado em inimigo, que � Object no Blackboard
			BlackBoardComp->SetValueAsObject("Inimigo", SensedPawn);
			//Se esta fun��o for chamada significa que o inimigo viu um jogador
			//A� � setado false no blackboard para que a �rvore de comportamento
			//leia e trate este valor.
			BlackBoardComp->SetValueAsBool("DevePerambular", false);
			//GetPawn() devolve um objeto gen�rico do tipo a Pawn

			//Toda vez que o Bot ver o player ele vai atirar
			//com sensibilidade .25 do sensor
			Bot->ArmaInimigo->Atirar();
		}
	}
	
	
}
