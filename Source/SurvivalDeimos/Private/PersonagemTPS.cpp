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
	//tamanho do braçco da spring
	SpringArmCamera->TargetArmLength = 200.f; 
	//usar rotacao do personagem
	SpringArmCamera->bUsePawnControlRotation = true; 
	SpringArmCamera->AddRelativeLocation(FVector(0.f, 40.f, 50.f));
	SpringArmCamera->bEnableCameraLag = true;
	SpringArmCamera->CameraLagSpeed = 40.f;
	SpringArmCamera->SetupAttachment(RootComponent);

	CameraPersonagem = CreateDefaultSubobject<UCameraComponent>(FName("CameraPersonagem"));
	CameraPersonagem->SetupAttachment(SpringArmCamera);

	//Indicando que o personagem poderá fazer o agachamento
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
//Movendo a cápsula no pulo
//UKismetSystemLibrary::MoveComponentTo(
//	GetCapsuleComponent(), // Aqui deve ser colocado o Componente a ser movido, neste caso a capsula
//	FVector(X, Y, Z), //Aqui a localização alvo, para qual local será movido o componente
//	RotCapsule, //Aqui a rotação alvo, que rotação deseja que o componente tenha ao ser movido(no nosso caso a mesma rotação da capsula sem alterações)
//	false, //se for true, será aliviado o movimento (ou seja, terminará lentamente) durante a movimentação
//	false, //se for true, o movimento começará devagar e depois fica mais rápido 
//	0.5f, //O Tempo do deslocamento do alvo... Neste caso coloquei 0.5 pois é +- o tempo do personagem pular ficando no ar
//	false, //se for true, será usado o caminho mais curto para rotação
//	EMoveComponentAction::Move, //Uma Enum que indica qual comportamento será efetuado, neste caso Movimentar (Move)
//	LatentInfo // A ação latente (são coisas bem específicas, mantenha o padrão do código)
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
	//Chamando função atirar da classe AArma
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
		//Busca o controlador do personagem e coloca o endereço no ponteiro PlayerController
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		//Macro para verificar se o ponteiro está Ok e evitar problemas de crash
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

	PlayerInputComponent->BindAction("Atirar", EInputEvent::IE_Pressed, this,
		&APersonagemTPS::Atirar);
}