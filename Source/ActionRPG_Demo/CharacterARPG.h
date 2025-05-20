// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterARPG.generated.h"

UCLASS()
class ACTIONRPG_DEMO_API ACharacterARPG : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackMontage;

	/** Socket name of the attack offset */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	FName AttackOffsetSocket;

	// Sets default values for this character's properties
	ACharacterARPG();

	/**
	* Performs attack animation when character attacks
	*/
	void PerformAttack();

	/**
	* Executes collision against enemy characters
	*/
	void PerformHit();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowprivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};
