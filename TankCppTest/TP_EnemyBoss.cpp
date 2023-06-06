// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_EnemyBoss.h"
#include "TP_Room.h"
#include "TP_TankEnemy.h"
#include "TP_BOSS_level3.h"
ATP_EnemyBoss::ATP_EnemyBoss()
{

}
void ATP_EnemyBoss::BeginPlay()
{
	Super::BeginPlay();
	canBeTarget = false;
	if (shieldComponent)
		shieldComponent->SetDamageable(false);
	if (roomOwner)
		roomOwner->OnRoomEnemyDestroy().AddDynamic(this, &ATP_EnemyBoss::DefenderDestroy);
	ATP_TankEnemy* _tank = Cast<ATP_TankEnemy>(this);
	ATP_BOSS_level3* _boss = Cast<ATP_BOSS_level3>(this);
	if (_tank && !_boss)
	{
		listOfProtectors.Add(this);
		generatorWithProtection = true;
	}
}
void ATP_EnemyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (shieldComponent && shieldComponent->IsDamageable() && !generatorWithProtection)
		CoolDownRestoreShield(DeltaTime);
}
void ATP_EnemyBoss::DefenderDestroy()
{

}
bool ATP_EnemyBoss::AreProtectorsDead()
{
	int _nbrOfProtectors = listOfProtectors.Num();
	for (int i = 0; i < _nbrOfProtectors; i++)
	{
		if (!listOfProtectors[i] || listOfProtectors[i]->GetHealthComponent()->IsDead())continue;
		return false;
	}
	return true;
}

void ATP_EnemyBoss::CoolDownRestoreShield(float _deltaTime)
{
	time += _deltaTime;
	if (time < timeDamageable || isDead)return;
	RestoreShield();
}

void ATP_EnemyBoss::RestoreShield()
{
	shieldComponent->SetDamageable(false);
	shieldComponent->AddShield(shieldComponent->GetMaxShield());
	canBeTarget = false;
	time = 0;
}
