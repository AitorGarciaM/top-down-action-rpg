// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterARPG.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACharacterARPG::ACharacterARPG()
{
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Configure Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 720.f, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	// Create Camera
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = true;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Performs attack animation when character attacks
void ACharacterARPG::PerformAttack()
{
	if (AttackMontage != nullptr)
	{
		PlayAnimMontage(AttackMontage);
		PerformHit();
	}
}

// Executes collision against enemy characters
void ACharacterARPG::PerformHit()
{
	FHitResult Hit;

	// Set line trace from the attack socket to a point 1000cm ahead of it.
	//FVector Start = GetMesh()->GetSocketTransform(AttackOffsetSocket, ERelativeTransformSpace::RTS_World).GetLocation();
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 1000.f; 

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Line trace will ignore this character when triggered

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, QueryParams);

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);

	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName()));
	}
}

// Called every frame
void ACharacterARPG::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

