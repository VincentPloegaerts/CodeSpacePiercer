#include "TP_TowerMissile.h"
#include "TP_Room.h"
#include "TP_RoomManager.h"
#include "TP_AIBehaviorComponent.h"
#include "TP_ShootTowerSystemComponent.h"
/*===========================================================
* DEPRECATED DO NOT USE
===========================================================*/
ATP_TowerMissile::ATP_TowerMissile()
{
	PrimaryActorTick.bCanEverTick = true;
	shootSystem = CreateDefaultSubobject<UTP_ShootTowerSystemComponent>(TEXT("ShootAiSystem"));
	towerBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	towerBase->SetupAttachment(RootComponent);
	GetMesh()->SetupAttachment(towerBase);
}

void ATP_TowerMissile::BeginPlay()
{
	Super::BeginPlay();
}

void ATP_TowerMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATP_TowerMissile::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
