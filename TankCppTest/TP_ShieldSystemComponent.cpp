#include "TP_ShieldSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "TP_Enemy.h"
#include "Components/SphereComponent.h"

#include "TP_Enemy.h"

#include "TP_TankPlayer.h"

UTP_ShieldSystemComponent::UTP_ShieldSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UTP_ShieldSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
	if (!world)return;
	owner = GetOwner();
	if (!owner)return;
	myShieldActor = world->SpawnActor<ATP_ShieldActor>(shields, owner->GetActorLocation(), FRotator::ZeroRotator);
	if (shield == 0 && myShieldActor)
		myShieldActor->HideShield();

	onDamage.AddDynamic(this, &UTP_ShieldSystemComponent::ResetShieldRegen);
	onRegen.AddDynamic(this, &UTP_ShieldSystemComponent::RegenShield);
	if (!myShieldActor)return;
	onShieldChange.AddDynamic(myShieldActor, &ATP_ShieldActor::SetShieldCrackVisibility);
	myShieldActor->AttachToActor(owner, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
	onRegen.AddDynamic(myShieldActor, &ATP_ShieldActor::ShowShield);
	onShieldDestroy.AddDynamic(myShieldActor, &ATP_ShieldActor::HideShield);

	ATP_Enemy* _enemy = Cast<ATP_Enemy>(owner);
	if (!_enemy)return;
	myShieldActor->SetShieldScale(_enemy->GetSphereScale());
}
void UTP_ShieldSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TimeBehaviour();
}

void UTP_ShieldSystemComponent::ResetShieldRegen()
{
	time = 0;
}
void UTP_ShieldSystemComponent::TakeDamage(float _damage)
{
	if (shield == 0)return;
	if (!isDamageable)return;
	if (_damage == -1)
		shield = 0;
	else
		shield -= _damage;
	shield = shield < 0 ? 0 : shield;
	onDamage.Broadcast();
	onShieldChange.Broadcast(shield, maxShield);
	if (shield == 0)
		onShieldDestroy.Broadcast();
	if (!myShieldActor)return;
	if (canFlash)
	{
		world->GetTimerManager().ClearTimer(timer);
		myShieldActor->SetShieldFlash(flashPower);
		world->GetTimerManager().SetTimer(timer, myShieldActor, &ATP_ShieldActor::ResetShieldFlash, flashTime, false);
	}
}
void UTP_ShieldSystemComponent::RegenShield()
{
	if (regenRate == 0 || shield >= maxShield)return;
	shield += regenRate * world->DeltaTimeSeconds;
	shield = shield > maxShield ? maxShield : shield;
	onShieldChange.Broadcast(shield, maxShield);
}
void UTP_ShieldSystemComponent::TimeBehaviour()
{
	if (regenRate == 0)return;
	if (shield == maxShield)return;
	time += world->DeltaTimeSeconds;
	if (!CanRegen())return;
	onRegen.Broadcast();
	ATP_Enemy* _enemy = Cast<ATP_Enemy>(owner);
	if (!_enemy)return;
	if (!myShieldActor)return;
	myShieldActor->SetShieldScale(_enemy->GetSphereScale());
}

void UTP_ShieldSystemComponent::AddShield(float _shieldValue)
{
	if (_shieldValue <= 0)return;
	shield += _shieldValue;
	shield = shield > maxShield ? maxShield : shield;
	onShieldChange.Broadcast(shield, maxShield);
	onRegen.Broadcast();
	ATP_Enemy* _enemy = Cast<ATP_Enemy>(owner);
	if (!_enemy)return;
	myShieldActor->SetShieldScale(_enemy->GetSphereScale());
}

void UTP_ShieldSystemComponent::DestroyShield()
{
	TakeDamage(maxShield);
}

void UTP_ShieldSystemComponent::DestroyShieldActor()
{
	if (!myShieldActor)return;
	myShieldActor->SetLifeSpan(0.5f);
}

void UTP_ShieldSystemComponent::SetDamageable(bool _isDamageable)
{
	isDamageable = _isDamageable;
}