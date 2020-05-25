// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeSurvivalDeimos.h"
#include "PersonagemTPS.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void AGameModeSurvivalDeimos::BeginPlay()
{
	Super::BeginPlay();
	//Aqui obtenho o personagem do jogo e coloco em Player
	APersonagemTPS* Player = Cast<APersonagemTPS>
		(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//Aqui se for setada algo como classe do HUD entramos
	if (PlayerHUDClass != nullptr)
	{
		//WidgetAtual passa a ser a widget criada (a tela com o health)
		WidgetAtual = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		//se for criada com sucesso a função addtoviewport é chamada
		//a widget é colocada na tela com a barra de health do jogador
		if (WidgetAtual)
		{
			WidgetAtual->AddToViewport();
		}
	}
}
