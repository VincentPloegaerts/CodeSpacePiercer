#include "TP_ShootMinigunSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "TP_Enemy.h"
#include "TP_TankPlayer.h"
#include "TP_LootBox.h"
#include "TP_LootPince.h"
#include "TP_HealthComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "TP_AIBehaviorComponent.h"
#include "NiagaraFunctionLibrary.h"

UTP_ShootMinigunSystemComponent::UTP_ShootMinigunSystemComponent()
{
}
void UTP_ShootMinigunSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	toIgnore.Add(owner);
	onHit.AddDynamic(this, &UTP_ShootMinigunSystemComponent::SwitchMaterial);
}
void UTP_ShootMinigunSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdatHeatJauge(DeltaTime);
}

void UTP_ShootMinigunSystemComponent::Shoot()
{
	if (!isActivated)return;
	if (isOverheated)
	{
		onStopShootEvent.Broadcast();
		return;
	}
	onShootEvent.Broadcast();
	ATP_TankPlayer* _owner = Cast<ATP_TankPlayer>(owner);
	if (!_owner)return;
	UCameraComponent* _camera = _owner->GetCameraComponent();
	FVector _forward = _camera->GetForwardVector();
	FVector _spawnLocation = _camera->GetComponentLocation() + _forward * 425;
	FVector _endLocation = _spawnLocation + _forward * attackRange;
	FHitResult _result;
	bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
	_spawnLocation,
	_endLocation,
	objectsLayer,
	false,
	toIgnore,
	EDrawDebugTrace::None,
	_result,
	true,
	FLinearColor::Red,
	FLinearColor::Green);
	if (!_hit)return;
	DealDammage(_result);

	onHit.Broadcast(_result);
	canShoot = false;
}
void UTP_ShootMinigunSystemComponent::DealDammage(FHitResult& _result)
{
	ATP_LootPince* _pince = Cast<ATP_LootPince>(_result.GetActor());
	if (_pince)
	{
		if (_result.ImpactPoint.Z < _pince->GetActorLocation().Z - 200)return;
		_pince->DropLootBox();
		return;
	}
	ATP_LootBox* _lootBox = Cast<ATP_LootBox>(_result.GetActor());
	if (_lootBox)
	{
		_lootBox->DestroyLootBox();
		return;
	}
	ATP_Enemy* _target = Cast<ATP_Enemy>(_result.GetActor());
	if (!_target)return;
	UTP_ShieldSystemComponent* _targetShield = _target->GetShieldComponent();
	if (_targetShield && !_targetShield->IsBroke())
	{
		_targetShield->TakeDamage(shieldDammage); 
		return;
	}
	UTP_HealthComponent* _targetHealth = _target->GetHealthComponent();
	if (!_targetHealth)return;
	_targetShield->ResetShieldRegen();
	_targetHealth->TakeDamage(healthDammage);
}
void UTP_ShootMinigunSystemComponent::UpdatHeatJauge(float _DeltaTime)
{
	if (isTriggerShoot && !isOverheated)
	{
		heatJauge += _DeltaTime;
		onHeatChange.Broadcast(heatJauge,heatJaugeMax);
	}
	if (!isTriggerShoot || isOverheated)
	{
		heatJauge -= (heatJaugeMax / coolingTime) * _DeltaTime;
		if (heatJauge <= 0)
		{
			heatJauge = 0;
			isOverheated = false;
			onOverheatStop.Broadcast();
		}
		onHeatChange.Broadcast(heatJauge, heatJaugeMax);
	}
	if (heatJauge >= heatJaugeMax)
	{
		onStopShootEvent.Broadcast();
		isOverheated = true;
	}
}
void UTP_ShootMinigunSystemComponent::SwitchMaterial(FHitResult result)
{
	ATP_Enemy* _target = Cast<ATP_Enemy>(result.GetActor());

	if (_target)
	{
		UTP_ShieldSystemComponent* _targetShield = _target->GetShieldComponent();
		if (_targetShield && !_targetShield->IsBroke() && _targetShield->IsDamageable())
		{
			if (allParticleOnImpact.Num() > 1)
			{
				if (!allParticleOnImpact[1])return;
				particleOnImpact = allParticleOnImpact[1];
			}
		}
		else if ( _targetShield && !_targetShield->IsDamageable() && !_targetShield->IsBroke())
		{
			if (allParticleOnImpact.Num() > 3)
			{
				if (!allParticleOnImpact[3])return;
				particleOnImpact = allParticleOnImpact[3];
			}
		}
		else
		{
			if (allParticleOnImpact.Num() > 0)
			{
				if (!allParticleOnImpact[0])return;
				particleOnImpact = allParticleOnImpact[0];
			}

		}
	}
	else
	{

		switch (UGameplayStatics::GetSurfaceType(result))
		{
		case EPhysicalSurface::SurfaceType1:
		{
			if (allParticleOnImpact.Num() > 0)
			{
				if (!allParticleOnImpact[0])return;
				particleOnImpact = allParticleOnImpact[0];
				break;
			}
		}
		case EPhysicalSurface::SurfaceType2:
		{
			if (allParticleOnImpact.Num() > 1)
			{
				if (!allParticleOnImpact[1])return;
				particleOnImpact = allParticleOnImpact[1];
				break;
			}
		}
		case EPhysicalSurface::SurfaceType3:
		{
			if (allParticleOnImpact.Num() > 2)
			{
				if (!allParticleOnImpact[2])return;
				particleOnImpact = allParticleOnImpact[2];
				break;
			}
		}
		case EPhysicalSurface::SurfaceType4:
		{
			if (allParticleOnImpact.Num() > 3)
			{
				if (!allParticleOnImpact[3])return;
				particleOnImpact = allParticleOnImpact[3];
				break;
			}
		}
		default:
		{
			if (allParticleOnImpact.Num() > 0)
			{
				if (!allParticleOnImpact[0])return;
				particleOnImpact = allParticleOnImpact[0];
				break;
			}
		}

		}
	}
	
	if (particleOnImpact)
	{
		FVector _normalVec = result.ImpactNormal;
		FRotator _normal = _normalVec.ToOrientationRotator() * -1;
		_normal.Yaw -= _normalVec.X * 180;
		for (size_t i = 0; i < particleNb; i++)
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, particleOnImpact, result.ImpactPoint + UKismetMathLibrary::RandomUnitVector() * dispersion, _normal);
	}
}