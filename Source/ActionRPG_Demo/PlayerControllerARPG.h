// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	/** Mapping Context */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Action */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationAction;
protected:
	void BenchmarkHitDetection();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

private:
	FVector CacheDestination;

	float PressTime; // How long it has been pressed.
};
