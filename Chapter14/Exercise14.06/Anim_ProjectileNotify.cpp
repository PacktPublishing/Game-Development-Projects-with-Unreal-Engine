// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim_ProjectileNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "SuperSideScroller/SuperSideScroller_Player.h"

void UAnim_ProjectileNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    ASuperSideScroller_Player* Player = Cast<ASuperSideScroller_Player>(MeshComp->GetOwner());
    if (Player)
    {
        Player->SpawnProjectile();
    }
}
