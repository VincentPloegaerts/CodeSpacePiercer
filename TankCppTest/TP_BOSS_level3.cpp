#include "TP_BOSS_level3.h"
#include "TP_Room.h"
#include "TP_AITankBehaviorComponent.h"

ATP_BOSS_level3::ATP_BOSS_level3()
{
	shootMissile = CreateDefaultSubobject< UTP_ShootTowerSystemComponent >(TEXT("ShootMissile"));
}

void ATP_BOSS_level3::BeginPlay()
{
	Super::BeginPlay();
	OnTankEnnemyTargetLost().AddDynamic(shootMissile, &UTP_ShootSystemComponent::ActivateShoot);
	OnTankEnnemyTargetAcquired().AddDynamic(shootMissile, &UTP_ShootSystemComponent::ActivateShoot);
	OnTankEnnemyAttack().AddDynamic(shootMissile, &UTP_ShootSystemComponent::ActivateShoot);
	UTP_AITankBehaviorComponent* _aiTank = Cast<UTP_AITankBehaviorComponent>(aiBehaviour);
	if (!_aiTank)return;
	_aiTank->OnStopNormalShooting().AddDynamic(shootMissile, &UTP_ShootSystemComponent::DesactivateShoot);
	_aiTank->OnNormalShooting().AddDynamic(shootMissile, &UTP_ShootSystemComponent::ActivateShoot);

	healthComponent->OnMidLife().RemoveDynamic(this, &ATP_BOSS_level3::RestoreShield);

	healthComponent->OnTwoThirdLife().AddDynamic(plasmaComponent, &UTP_ShootPlasmaComponent::ShootAOEPlasma);
	healthComponent->OnOneThirdLife().AddDynamic(plasmaComponent, &UTP_ShootPlasmaComponent::ShootAOEPlasma);
	
	healthComponent->OnTwoThirdLife().AddDynamic(this, &ATP_BOSS_level3::Steroid);
	healthComponent->OnOneThirdLife().AddDynamic(this, &ATP_BOSS_level3::Steroid);

	if (shieldComponent)
		shieldComponent->SetDamageable(false);
	shootMissile->SetMeshShoot(meshRadar);
}

void ATP_BOSS_level3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ATP_BOSS_level3::Activate()
{
	Super::Activate();
	if (shieldComponent)
		shieldComponent->SetDamageable(false);
	canBeTarget = false;
}
void ATP_BOSS_level3::DefenderDestroy()
{
	if (!roomOwner)return;
	if (generatorWithProtection && AreProtectorsDead())
	{
		GetShieldComponent()->SetDamageable(true);
		GetShieldComponent()->DestroyShield();
		canBeTarget = true;
		return;
	}
}
void ATP_BOSS_level3::Init(UWorld* _world)
{
	Super::Init(_world);
	ATP_Room* _owner = GetRoomOwner();
	if (!_owner)return;
	_owner->AddToBossArray(this);
}

void ATP_BOSS_level3::Steroid()
{

	DescativateShoot();
	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &ATP_BOSS_level3::ActivateShoot, timerNoAttackAfterBoost,false);

}

void ATP_BOSS_level3::DescativateShoot()
{
	shootMissile->DesactivateShoot();
	aiMinigunShootComponent->DesactivateShoot();
	shootMissile->SetIsActivated(false);
	aiMinigunShootComponent->SetIsActivated(false);
}

void ATP_BOSS_level3::ActivateShoot()
{

	shootMissile->SetRate(shootMissile->GetRate() * missileRateBoost);
	shootMissile->SetBoost(missileSpeedBoost);
	aiMinigunShootComponent->SetShootRate(aiMinigunShootComponent->GetShootRate() * miniGunBoost);

	shootMissile->SetIsActivated(true);
	aiMinigunShootComponent->SetIsActivated(true);
	shootMissile->ActivateShoot();
	aiMinigunShootComponent->ActivateShoot();
}

void ATP_BOSS_level3::ActorDeath(AActor* _deadActor)
{
	
	ATP_BOSS_level3* _deadBoss = Cast<ATP_BOSS_level3>(_deadActor);
	if (!_deadBoss || !_deadBoss->GetRoomOwner())return;
	_deadBoss->GetRoomOwner()->RemoveBoss(_deadBoss);
	Super::ActorDeath(_deadBoss);
}