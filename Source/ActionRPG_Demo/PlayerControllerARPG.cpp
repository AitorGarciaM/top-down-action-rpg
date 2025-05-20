// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerARPG.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "InputActionValue.h"

APlayerControllerARPG::APlayerControllerARPG()
{
	CacheDestination = FVector::ZeroVector;
	PressTime = 0.f;
}

void APlayerControllerARPG::BenchmarkHitDetection()
{
	const int32 NumInterations = 10000;
	float TotalTimeUnderCursor = 0.0f;
	float TotalTimeLinetrace = 0.0f;

	FVector2D MousePosition(500.f, 500.f); // Simulate Mouse position.
	FHitResult HitResult;

	// Benchmark GetHitResultUnderCursor
	{
		double Start = FPlatformTime::Seconds();
		for (int32 i = 0; i < NumInterations; i++)
		{
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
		}
		double End = FPlatformTime::Seconds();
		TotalTimeUnderCursor = End - Start;
	}

	// Benchmark LineTrace
	{
		FVector WorldLocation, WorldDirection;
		DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);

		FVector TraceStart = WorldLocation;
		FVector TraceEnd = TraceStart + (WorldDirection * 10000.0f);

		double Start = FPlatformTime::Seconds();
		for (int32 i = 0; i < NumInterations; i++)
		{
			GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility);
		}
		double End = FPlatformTime::Seconds();
		TotalTimeLinetrace = End - Start;
	}

	UE_LOG(LogTemp, Warning, TEXT("GetHitResultUnderCursor: %f sec for %d iterations"), TotalTimeUnderCursor, NumInterations);
	UE_LOG(LogTemp, Warning, TEXT("LineTraceSingleByChannel: %f sec for %d iterations"), TotalTimeLinetrace, NumInterations);
}

void APlayerControllerARPG::BeginPlay() 
{
	Super::BeginPlay();
	// Enable Cursos on window
	bShowMouseCursor = true;

	CharacterARPG = Cast<ACharacterARPG>(GetCharacter());
}

void APlayerControllerARPG::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse left click.
		EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Started, this, &APlayerControllerARPG::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Triggered, this, &APlayerControllerARPG::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Completed, this, &APlayerControllerARPG::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Canceled, this, &APlayerControllerARPG::OnSetDestinationReleased);

		// Setup mouse right click.
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerControllerARPG::OnInputAttack);
	}
}

// Reset the movement on change desired destination
void APlayerControllerARPG::OnInputStarted()
{
	StopMovement();
}

void APlayerControllerARPG::OnSetDestinationTriggered()
{
	PressTime += GetWorld()->GetDeltaSeconds();

	FHitResult Hit;
	bool bHitSucceded = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	if (bHitSucceded)
	{
		CacheDestination = Hit.Location;
	}

	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CacheDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

// Set destination with single click
void APlayerControllerARPG::OnSetDestinationReleased()
{
	if (PressTime <= ShortPressThreshold)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CacheDestination);
	}

	PressTime = 0.f;
}

void APlayerControllerARPG::OnInputAttack()
{	
	if (!b_isAttacking)
	{
		b_isAttacking = true;

		FHitResult Hit;
		bool bHitSucceded = GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);

		if (bHitSucceded)
		{
			// Rotates Character to mouse position
			FVector TargetLocation = Hit.Location;
			FVector ActorLocation = GetPawn()->GetActorLocation();

			TargetLocation.Z = ActorLocation.Z;

			FVector Direction = (TargetLocation - ActorLocation).GetSafeNormal();

			if (!Direction.IsNearlyZero())
			{
				FRotator TargetRotator = Direction.Rotation();
				GetPawn()->SetActorRotation(TargetRotator);
			}

			StopMovement();
			OnAttack();
		}
	}
}

void APlayerControllerARPG::OnAttack()
{
	FTimerHandle AttackTimerHandle;
	
	if (CharacterARPG != nullptr)
	{
		// Performs Attack Animation
		CharacterARPG->PerformAttack();
	}

	// Starts the attack cooldown timer.
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &APlayerControllerARPG::ResetAttack, AttackCooldown, false);
}

void APlayerControllerARPG::ResetAttack()
{
	b_isAttacking = false;
}
