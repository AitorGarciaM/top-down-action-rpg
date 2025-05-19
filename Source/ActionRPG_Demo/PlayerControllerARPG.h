// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterARPG.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerARPG.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class ACTIONRPG_DEMO_API APlayerControllerARPG : public APlayerController
{
	GENERATED_BODY()
public:
	APlayerControllerARPG();

	/** Time Threshold to know if it was short press */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** Waiting Time between attacks */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	float AttackCooldown;

	/** Mapping Context */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Action */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationAction;

	/** Attack Action */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

protected:
	void BenchmarkHitDetection();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	void OnInputAttack();

	void OnAttack();
	void ResetAttack();

private:
	class ACharacterARPG* CharacterARPG;
	FVector CacheDestination;

	float PressTime; // How long it has been pressed.
	float AttackTime; // How long it has performed.

	bool b_isAttacking;
};
