// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonagemTPS.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Arma.h"
#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/Public/WorldCollision.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/LatentActionManager.h"

// Sets default values
APersonagemTPS::APersonagemTPS()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmCamera = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmCamera"));
	//tamanho do bra�co da spring
	SpringArmCamera->TargetArmLength = 200.f; 
	//usar rotacao do personagem
	SpringArmCamera->bUsePawnControlRotation = true; 
	SpringArmCamera->AddRelativeLocation(FVector(0.f, 40.f, 50.f));
	SpringArmCamera->bEnableCameraLag = true;
	SpringArmCamera->CameraLagSpeed = 40.f;
	SpringArmCamera->SetupAttachment(RootComponent);

	CameraPersonagem = CreateDefaultSubobject<UCameraComponent>(FName("CameraPersonagem"));
	CameraPersonagem->SetupAttachment(SpringArmCamera);

	//Indicando que o personagem poder� fazer o agachamento
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCharacterMovement()->AirControl = 3.f;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->CrouchedHalfHeight = 70.f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APersonagemTPS::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters Parametros;
	Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	ArmaPlayer = GetWorld()->SpawnActor<AArma>(BP_ArmaTipoRifle, FTransform(), Parametros);
	ArmaPlayer->AttachToComponent(Cast<USceneComponent>(GetMesh()),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		("SocketDaArma"));
	
}

void APersonagemTPS::Pular()
{
	bEstaPulando = true;
	float X = GetCapsuleComponent()->GetRelativeLocation().X;
	float Y = GetCapsuleComponent()->GetRelativeLocation().Y + 40;
	float Z = GetCapsuleComponent()->GetRelativeLocation().Z + 100;
	FRotator RotCapsule = GetCapsuleComponent()->GetRelativeRotation();
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.UUID = GetUniqueID();
	LatentInfo.ExecutionFunction = "Jump";
	LatentInfo.Linkage = 0;
	UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), FVector(X, Y, Z), RotCapsule, false, false, 0.5f, false, EMoveComponentAction::Move, LatentInfo);
	GetCapsuleComponent()->SetCapsuleHalfHeight(70.f);
}
//Movendo a c�psula no pulo
//UKismetSystemLibrary::MoveComponentTo(
//	GetCapsuleComponent(), // Aqui deve ser colocado o Componente a ser movido, neste caso a capsula
//	FVector(X, Y, Z), //Aqui a localiza��o alvo, para qual local ser� movido o componente
//	RotCapsule, //Aqui a rota��o alvo, que rota��o deseja que o componente tenha ao ser movido(no nosso caso a mesma rota��o da capsula sem altera��es)
//	false, //se for true, ser� aliviado o movimento (ou seja, terminar� lentamente) durante a movimenta��o
//	false, //se for true, o movimento come�ar� devagar e depois fica mais r�pido 
//	0.5f, //O Tempo do deslocamento do alvo... Neste caso coloquei 0.5 pois � +- o tempo do personagem pular ficando no ar
//	false, //se for true, ser� usado o caminho mais curto para rota��o
//	EMoveComponentAction::Move, //Uma Enum que indica qual comportamento ser� efetuado, neste caso Movimentar (Move)
//	LatentInfo // A a��o latente (s�o coisas bem espec�ficas, mantenha o padr�o do c�digo)
//);

void APersonagemTPS::PararPulo()
{
	bEstaPulando = false;
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
}

void APersonagemTPS::Agachar()
{
	Crouch();
}

void APersonagemTPS::Levantar()
{
	UnCrouch();
}

void APersonagemTPS::Atirar()
{
	//Chamando fun��o atirar da classe AArma
	ArmaPlayer->Atirar();
}

void APersonagemTPS::SetHealth(float Dano)
{
	if (Health > 0.0f)
	{
		Health -= Dano;
	}
	else if (Health <= 0.0f)
	{
		bEstaMorto = true;
		//Busca o controlador do personagem e coloca o endere�o no ponteiro PlayerController
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		//Macro para verificar se o ponteiro est� Ok e evitar problemas de crash
		check(PlayerController);
		//Com o objeto PlayerController buscamos o Player via GetPawn e
		//depois desabilitamos o Input deste Jogador para evitar que ele se mova
		//ao morrer
		PlayerController->GetPawn()->DisableInput(PlayerController);
	}
	
}

float APersonagemTPS::GetHealth()
{
	return Health;
}

bool APersonagemTPS::IsEstaMorto()
{
	return bEstaMorto;
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
	//Amarrar MoverParaFrente a fun��o
	PlayerInputComponent->BindAxis("MoverParaFrente", this,
		&APersonagemTPS::MoverParaFrente);
	//Amarrar MoverDireita a fun��o
	PlayerInputComponent->BindAxis("MoverDireita", this,
		&APersonagemTPS::MoverDireita);
	//Rota��o do personagem
	PlayerInputComponent->BindAxis("OlharCimaBaixo", this,
		&APersonagemTPS::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("OlharDireitaEsquerda", this,
		&APersonagemTPS::AddControllerYawInput);

	//EInputEvent::IE_Pressed - bot�o pessionado
	PlayerInputComponent->BindAction("Agachar", EInputEvent::IE_Pressed, this,
		&APersonagemTPS::Agachar);
	//EInputEvent::IE_Released - Soltar bot�o
	PlayerInputComponent->BindAction("Agachar", EInputEvent::IE_Released, this,
		&APersonagemTPS::Levantar);

	PlayerInputComponent->BindAction("Pular", EInputEvent::IE_Pressed, this,
		&APersonagemTPS::Pular);
	PlayerInputComponent->BindAction("Pular", EInputEvent::IE_Released, this,
		&APersonagemTPS::PararPulo);

	PlayerInputComponent->BindAction("Atirar", EInputEvent::IE_Pressed, this,
		&APersonagemTPS::Atirar);
}