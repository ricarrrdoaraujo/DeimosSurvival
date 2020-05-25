// Fill out your copyright notice in the Description page of Project Settings.


#include "BotCharacter.h"
#include "Engine/World.h"
#include "Arma.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABotCharacter::ABotCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABotCharacter::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters Parametros;
	Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ArmaInimigo = GetWorld()->SpawnActor<AArma>(BP_ArmaTipoRifle, FTransform(), Parametros);
	ArmaInimigo->AttachToComponent(Cast<USceneComponent>(GetMesh()),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		("SocketDaArma"));
	
}

// Called every frame
void ABotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABotCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABotCharacter::SetHealth(float Dano)
{
	if (Health > 0.0f)
	{
		Health -= Dano;
	}
	else if (Health <= 0.0f)
	{
		bEstaMorto = true;
		//Inimigo vai ter 10 segundos de vida
		SetLifeSpan(10.f);
		//Destuir a marma quando ele morrer
		ArmaInimigo->Destroy();
	}
}

float ABotCharacter::GetHealth()
{
	return Health;
}

bool ABotCharacter::IsEstaMorto()
{
	return bEstaMorto;
}

