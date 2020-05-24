// Fill out your copyright notice in the Description page of Project Settings.


#include "Arma.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/Public/CollisionQueryParams.h"
#include "Engine/World.h"
#include "Engine/Public/DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AArma::AArma()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EfeitoMuzzle = nullptr;

	MalhaDaArma = CreateDefaultSubobject<USkeletalMeshComponent>(FName("MalhaDaArma"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshDaArma(TEXT("SkeletalMesh'/Game/Weapons/Rifle.Rifle'"));

	if (MeshDaArma.Succeeded())
	{
		//Setando que a malha da arma vai ser MeshDaArma
		MalhaDaArma->SetSkeletalMesh(MeshDaArma.Object);
	}

	RootComponent = MalhaDaArma;

	SetaDaArma = CreateDefaultSubobject<UArrowComponent>(FName("SetaCanoDaArma"));
	SetaDaArma->SetupAttachment(MalhaDaArma, FName("MuzzleFlashSocket"));

	//Location em rela��o a malha da arma
	SetaDaArma->SetRelativeLocation(FVector(1.5f, 0.f, -1.2f));
	SetaDaArma->SetRelativeScale3D(FVector(0.3f, 0.8f, 0.7f));
}

// Called when the game starts or when spawned
void AArma::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArma::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//Alcance do Raycast = Vetor de in�cio da seta(onde come�a o raycast) + vetor da dire��o * 1000
void AArma::Atirar()
{
	//UArrowComponent* SetaDaArma = FindComponentByClass<UArrowComponent>();
	if (SetaDaArma != nullptr)
	{
		//Seta da Arma � um componente do ator, logo para achar sua localiza��o
		//utilizamos GetComponentLocation
		//Se fosse a Arma seria GetActorLocation() pois a arma � o ator
		//que possui um arrow(SetaDaArma) como componente
		FVector Inicio = SetaDaArma->GetComponentLocation();
		//GetComponentRotation retorna um FRotator, a Unreal fornece o .Vector() que
		//automaticamente converte para FVector
		FVector Direcao = SetaDaArma->GetComponentRotation().Vector();
		FVector Fim = Inicio + (Direcao * 1000);

		/*O Raio de Raycast vai percorrer at� o fim e precisa colocar as
		informa��es de Hit em alguma estrutura. Esta estrutura � a FHitResult*/
		FHitResult InfoImpacto;
		FCollisionQueryParams Parametros;
		//Para o raio n�o colidir com a pr�pria seta (this)
		Parametros.AddIgnoredActor(this);
		//GetOwner devolve o dono da seta
		//Neste caso devolve a malha(mesh) da arma
		//tamb�m n�o queremos que a malha da arma seja considerada
		//na colis�o com o raio de raycast
		Parametros.AddIgnoredActor(GetOwner());
		Parametros.bTraceComplex = true;

		bool AcertouEmAlgo = GetWorld()->LineTraceSingleByChannel(InfoImpacto,
			Inicio, Fim, ECollisionChannel::ECC_Visibility, Parametros);

		if (AcertouEmAlgo)
		{
			UE_LOG(LogTemp, Warning, TEXT("Acertou em Algo"));
		}

		DrawDebugLine(GetWorld(), Inicio, Fim, FColor::Red, false, 5.0f, (uint8)0, 1.0f);

		//Se for setado algum efeito muzzle na blueprint da arma, EfeitoMuzzle
		//receber� um endere�o de mem�ria

		if (EfeitoMuzzle)
		{
			//Para Componentes usamos estas fun��es GetComponent, j� para atores seriam
			//GetActorLocation
			FVector Localizacao = SetaDaArma->GetComponentLocation();
			FRotator Rotacao = SetaDaArma->GetComponentRotation();
			//Se valores forem igual podemos inicial com um �nico par�metro
			FVector Escala = FVector(0.9f);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EfeitoMuzzle, Localizacao, Rotacao, Escala, true);
		}
	}
}

