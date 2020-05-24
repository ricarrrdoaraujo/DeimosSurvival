// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arma.generated.h"

UCLASS()
class SURVIVALDEIMOS_API AArma : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArma();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*meta = (AllowPrivateAccess = "true") Indica que esssa propriedade 
	estará acessível para acesso na blueprint
	Metadata AllowPrivateAccess está sendo usado para que componentes
	desta classe possam ser modificados na blueprints
	Assim poderemos modificar a malha da arma por exemplo*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arma", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* MalhaDaArma;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arma", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* SetaDaArma;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Atirar();

};
