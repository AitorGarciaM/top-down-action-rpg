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
	/** How fast will rotate to correct orientation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	float RotationSpeed = 5.f;

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

	/**
	* Moves player character to new destination
	*/
	void OnSetDestinationTriggered();

	void OnSetDestinationReleased();

	/**
	* Manage attack input
	*/
	void OnInputAttack();

	void OnAttack();
	void ResetAttack();

private:
	FTimerHandle RotatiomTimeHandle;
	FRotator TargetRotation;

	class ACharacterARPG* CharacterARPG;
	FVector CacheDestination;

	float PressTime; // How long it has been pressed
	float AttackTime; // How long it has performed

	bool b_isAttacking;

	void RotateToDirectionSmooth(const FVector& TargetDirection);
	void UpdateRotationSmooth();
};
