#include "TP_HealthComponent.h"
#include "TP_GameCharacter.h"
#include "TP_Enemy.h"

UTP_HealthComponent::UTP_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UTP_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	ATP_GameCharacter* _owner = Cast<ATP_GameCharacter>(GetOwner());
	if (!_owner)return;
	onDeathEvent.AddDynamic(_owner, &ATP_GameCharacter::ActorDeath);
}
void UTP_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTP_HealthComponent::TakeDamage(float _damage)
{
	if (healthStats.health <= 0)return;
	healthStats.health -= _damage;
	healthStats.health = healthStats.health < 0 ? 0 : healthStats.health;
	if (healthStats.health <= healthStats.maxHealth / 3 && !oneThird)
	{
		oneThird = true;
		onOneThirdLife.Broadcast();
	}
	if (healthStats.health <= (2*healthStats.maxHealth) / 3 && !twoThird)
	{
		twoThird = true;
		onTwoThirdLife.Broadcast();
	}
	if (healthStats.health <= healthStats.maxHealth / 2 && !midLife)
	{
		midLife = true;
		onMidLife.Broadcast(midLife);
	}
	onTakeDamage.Broadcast();
	onLifeChange.Broadcast(healthStats.health, healthStats.maxHealth);
	if (healthStats.health <= 0)
	{
		AActor* _owner = GetOwner();
		if (!_owner)return;
		onDeathEvent.Broadcast(_owner);
		return;
	}
}
void UTP_HealthComponent::AddStat(float _health)
{
	healthStats.health += _health;
	healthStats.health = healthStats.health >= healthStats.maxHealth ? healthStats.maxHealth : healthStats.health;
	if (healthStats.health > healthStats.maxHealth/2)
	{
		midLife = false;
	}
	onLifeChange.Broadcast(healthStats.health, healthStats.maxHealth);
}

void UTP_HealthComponent::KillActor()
{
	TakeDamage(healthStats.maxHealth);
}

