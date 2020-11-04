// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim_ProjectileNotify.h"

void UAnim_ProjectileNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    UE_LOG(LogTemp, Warning, TEXT("Throw Notify"));
}
