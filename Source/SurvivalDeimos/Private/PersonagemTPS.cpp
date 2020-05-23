// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonagemTPS.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"


// Sets default values
APersonagemTPS::APersonagemTPS()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmCamera = CreateDefaultSubobject<USpringArmComponent>(FName("ApringArmCamera"));
	//tamanho do braçco da spring
	SpringArmCamera->TargetArmLength = 200.f; 
	//usar rotacao do personagem
	SpringArmCamera->bUsePawnControlRotation = true; 
	SpringArmCamera->AddRelativeLocation(FVector(0.f, 40.f, 50.f));
	SpringArmCamera->SetupAttachment(RootComponent);

	CameraPersonagem = CreateDefaultSubobject<UCameraComponent>(FName("CameraPersonagem"));
	CameraPersonagem->SetupAttachment(SpringArmCamera);

	//Indicando que o personagem poderá fazer o agachamento
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCharacterMovement()->AirControl = 0.05f;
	GetCharacterMovement()->JumpZVelocity = 425.f;
	GetCharacterMovement()->GravityScale = 1.5f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APersonagemTPS::BeginPlay()
{
	Super::BeginPlay();
	
}

void APersonagemTPS::Pular()
{
	bEstaPulando = true;
}

void APersonagemTPS::PararPulo()
{
	bEstaPulando = false;
}

void APersonagemTPS::Agachar()
{
	Crouch();
}

void APersonagemTPS::Levantar()
{
	UnCrouch();
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

	//EInputEvent::IE_Pressed - botão pessionado
	PlayerInputComponent->BindAction("Agachar", EInputEvent::IE_Pressed, this,
		&APersonagemTPS::Agachar);
	//EInputEvent::IE_Released - Soltar botão
	PlayerInputComponent->BindAction("Agachar", EInputEvent::IE_Released, this,
		&APersonagemTPS::Levantar);

	PlayerInputComponent->BindAction("Pular", EInputEvent::IE_Pressed, this,
		&APersonagemTPS::Pular);
	PlayerInputComponent->BindAction("Pular", EInputEvent::IE_Released, this,
		&APersonagemTPS::PararPulo);
}