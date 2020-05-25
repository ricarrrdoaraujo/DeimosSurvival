// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PersonagemTPS.generated.h"

UCLASS()
class SURVIVALDEIMOS_API APersonagemTPS : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APersonagemTPS();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void MoverParaFrente(float Valor);

	void MoverDireita(float Valor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraPersonagem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArmCamera;

	//vari�vel capaz de receber qualquer tipo de
	//endere�o de classe de Arma ou subclasse de Arma
	UPROPERTY(EditAnywhere, Category = "Arma")
	TSubclassOf<class AArma> BP_ArmaTipoRifle;

	UPROPERTY(BlueprintReadOnly, Category = "Pulo")
	bool bEstaPulando;

	void Pular();

	void PararPulo();

	//Crouch
	void Agachar();

	//Unchrouch
	void Levantar();

	//Faz com que a arma atire
	void Atirar();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Health")
	float Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bEstaMorto;

public:	
	class AArma* ArmaPlayer;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void SetHealth(float Dano);

	UFUNCTION()
	float GetHealth();
};
