// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackHitCheck.h"
#include "GameFramework/Actor.h"
#include "CharacterARPG.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}

	ACharacterARPG* Character = Cast<ACharacterARPG>(MeshComp->GetOwner());
	if (Character)
	{
		Character->PerformHit();
	}
}
