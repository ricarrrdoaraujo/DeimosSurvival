// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonagemTPS.h"
#include "Components/InputComponent.h"

// Sets default values
APersonagemTPS::APersonagemTPS()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APersonagemTPS::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APersonagemTPS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void APersonagemTPS::MoverParaFrente(float Valor)
{
	//Obter vetor a frente desse personagem * Valor
	AddMovementInput(GetActorForwardVector() * Valor);
}

void APersonagemTPS::MoverDireita(float Valor)
{
	//Obter vetor direita desse personagem * Valor
	AddMovementInput(GetActorRightVector() * Valor);
}

// Called to bind functionality to input
void APersonagemTPS::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Amarrar MoverParaFrente a função
	PlayerInputComponent->BindAxis("MoverParaFrente", this,
		&APersonagemTPS::MoverParaFrente);
	//Amarrar MoverDireita a função
	PlayerInputComponent->BindAxis("MoverDireita", this,
		&APersonagemTPS::MoverDireita);
	//Rotação do personagem
	PlayerInputComponent->BindAxis("OlharCimaBaixo", this,
		&APersonagemTPS::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("OlharDireitaEsquerda", this,
		&APersonagemTPS::AddControllerYawInput);
}