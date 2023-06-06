// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_Door.h"
#include "DrawDebugHelpers.h"
#include "TP_TankPlayer.h"
#include "Components/BoxComponent.h"
#include "TP_GameMode.h"

#pragma region Engine Methods
ATP_Door::ATP_Door()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	NiagaraSide1 = CreateDefaultSubobject<UNiagaraComponent>("NiagaraSide1");
	NiagaraSide2 = CreateDefaultSubobject<UNiagaraComponent>("NiagaraSide 2");
	triggerBox = CreateDefaultSubobject<UBoxComponent>("Trigger Box");
	collisionBox = CreateDefaultSubobject<UBoxComponent>("Collision Box");

	if (NiagaraSide1)NiagaraSide1->SetupAttachment(RootComponent);
	if (NiagaraSide2)NiagaraSide2->SetupAttachment(RootComponent);
	if (triggerBox)triggerBox->SetupAttachment(RootComponent);
	if (collisionBox)collisionBox->SetupAttachment(RootComponent);

}
void ATP_Door::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ATP_Door::EnterBox);
	OnActorEndOverlap.AddDynamic(this, &ATP_Door::LeaveBox); 
	onDoorExplode.AddDynamic(this, &ATP_Door::ExplodeDoor);
	if (isOpen)
		OpenDoor();
	else
		CloseDoor();
}
void ATP_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isLerping)UpdateLerpValue(DeltaTime);
	if (!GetWorld()->GetAuthGameMode<ATP_GameMode>()->GetDebugDraw())return;
	DrawDebugDoor(GetWorld());
}

#pragma endregion


/*
* methods to activate and desactivate niagaras and collision
* also switch to an explosion when door opens up
*/
#pragma region Collision Box Methods
void ATP_Door::OpenDoor()
{
	isOpen = true;
	if (NiagaraSide1)NiagaraSide1->SetHiddenInGame(true);
	if (NiagaraSide2)NiagaraSide2->SetHiddenInGame(true);

	if (collisionBox)collisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
}
void ATP_Door::CloseDoor()
{
	isOpen = false;
	if (NiagaraSide1)NiagaraSide1->SetHiddenInGame(false);
	if (NiagaraSide2)NiagaraSide2->SetHiddenInGame(false);
	if (collisionBox)collisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void ATP_Door::ExplodeDoor()
{
	SetLifeSpan(2);
	if (collisionBox)collisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if (NiagaraSide1 && NiagaraExplosion)
		NiagaraSide1->SetAsset(NiagaraExplosion);
	if (NiagaraSide2 && NiagaraExplosion)
		NiagaraSide2->SetAsset(NiagaraExplosion);
}

#pragma endregion

/*
* just a debug when there wasn't anyvisuals
*/
#pragma region Debug
void ATP_Door::DrawDebugDoor(UWorld* _world)
{
	if (!_world)return;
	DrawDebugBox(_world, GetActorLocation(), triggerBox->GetScaledBoxExtent(), FColor::Green, false, -1, 0, 5);
	DrawDebugBox(_world, GetActorLocation(), collisionBox->GetScaledBoxExtent(), FColor::Red, false, -1, 0, 5);
}
#pragma endregion

/*
* Methods used to show the lock when approaching
* 
*/
#pragma region Lock methods
void ATP_Door::LeaveBox(AActor* _box, AActor* _other)
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(_other);
	if (!_player)return;
	isLerping = true;
	isInside = false;
}
void ATP_Door::EnterBox(AActor* _box, AActor* _other)
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(_other);
	if (!_player)return;
	isLerping = true;
	isInside = true;
}

void ATP_Door::UpdateLerpValue(float DeltaTime)
{
	if (!isLerping)return;
	lockLerpTime += DeltaTime;
	if (isInside)
		lockLerpValue = FMath::Lerp<float>(0, 1, lockLerpTime / lockLerpTimeMax);
	else 
		lockLerpValue = FMath::Lerp<float>(1, 0, lockLerpTime / lockLerpTimeMax);
	if (lockLerpValue >= 1 || lockLerpValue <= 0)
	{
		lockLerpValue = FMath::RoundToInt(lockLerpValue);
		isLerping = false;
		lockLerpTime = 0;
	}
	SetLockNiagaras(lockLerpValue);
}
void ATP_Door::SetLockNiagaras(float const _newValue)
{
	if (NiagaraSide1)
		NiagaraSide1->SetNiagaraVariableFloat(lockOpacityName, _newValue);
	if (NiagaraSide2)
		NiagaraSide2->SetNiagaraVariableFloat(lockOpacityName, _newValue);
}

#pragma endregion
