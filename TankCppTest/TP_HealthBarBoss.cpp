#include "TP_HealthBarBoss.h"
#include "TP_Generator.h"

void UTP_HealthBarBoss::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTP_HealthBarBoss::SetGeneratorOwner(ATP_Generator* _owner)
{
	owner = _owner;

	if (!owner)return;
	//owner->GetHealthComponent()->OnLifeChange().AddDynamic(this, &UTP_HealthBarBoss::SetHealth);
}

void UTP_HealthBarBoss::SetHealth(float _currentHealth, float _maxHealth)
{
	float _ratioHealth = _currentHealth/ _maxHealth;
	//if (!lifeBar)return;
	//lifeBar->SetPercent(_ratioHealth);
}

void UTP_HealthBarBoss::EraseTheWidget()
{
	RemoveFromParent();
}
