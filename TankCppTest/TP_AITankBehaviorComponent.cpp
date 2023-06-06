#include "TP_AITankBehaviorComponent.h"
#include "TP_TankEnemy.h"
#include "TP_TankPlayer.h"
#include "DrawDebugHelpers.h"
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>

UTP_AITankBehaviorComponent::UTP_AITankBehaviorComponent()
{
}

void UTP_AITankBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
	worldS = GetWorld();
	ownerTank = Cast<ATP_TankEnemy>(GetOwner());
	if (!ownerTank)
	{
		return;
	}
	//worldS->GetTimerManager().SetTimer(timerCheckDistance, this, &UTP_AITankBehaviorComponent::setCurrentDistance, reactionTime, true);

	ownerTank->bUseControllerRotationYaw = false;
	ownerTank->GetCharacterMovement()->bOrientRotationToMovement = true;
	ownerTank->GetCharacterMovement()->RotationRate.Yaw = ownerTank->GetCharacterMovement()->RotationRate.Yaw / 4;

	plasmaCurentCharge = ownerTank->GetPlasmaCurrentRange();
	plasmaMaxRange = ownerTank->GetPlasmaMaxRange();

	plasmaMaxRange->SetWorldScale3D(FVector(plasmaMaxRange->DecalSize.X / 100, plasmaMaxRange->DecalSize.Y / 50, plasmaMaxRange->DecalSize.Z/50));
	plasmaCurentCharge->SetWorldScale3D(FVector(plasmaCurentCharge->DecalSize.X / 100, 0, 0));
}

void UTP_AITankBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (isDead || !owner->IsCharacterActive())return;
	PlasmaTimerUp();

	FRotator _rota = FRotator::ZeroRotator;
	_rota.Pitch += 90;
	_rota.Roll -= 90;

	_rota += ownerTank->GetMeshTurret()->GetComponentRotation();
	if (!plasmaCurentCharge || !plasmaMaxRange)return;
	plasmaMaxRange->SetWorldRotation(_rota);
	plasmaCurentCharge->SetWorldRotation(_rota);

	plasmaMaxRange->SetWorldLocation(owner->GetActorLocation() + FVector(0, 0, -200));
	plasmaCurentCharge->SetWorldLocation(owner->GetActorLocation() + FVector(0, 0, -200));
	if (!retracting)return;
	timeRetracting += DeltaTime;
	if (timeRetracting >= timeBeforeRetracting)
	{
		onStopShootingPlasma.Broadcast();
		retracting = false;
		timeRetracting = 0;
	}
}

void UTP_AITankBehaviorComponent::AiBehaviour()
{
	Super::AiBehaviour();
	if (!owner || !target || isDead)return;
	float _distance = FVector::Distance(target->GetActorLocation(), owner->GetActorLocation());
	if (timePlasmaActivation >= timePlasmaActivationMax)
	{
		retracting = true;
		timePlasmaActivation = 0;
		ownerTank->GetPlasmaComponent()->Shoot();
	}
	if (_distance > RangePlasmaActivation && timePlasmaActivation < tresholdPlasmaWillContinue && _distance <= rangeAttack)
	{
		if (!retracting) onStopShootingPlasma.Broadcast();
		onNormalShooting.Broadcast();
		timePlasmaActivation = 0;
		ownerTank->GetMinigunComponent()->Shoot(); //maybe add a timer for shooting like if it starts then it continues for like 3 sec
	}
}

void UTP_AITankBehaviorComponent::Activation()
{
	worldS->GetTimerManager().SetTimer(timerCheckDistance, this, &UTP_AITankBehaviorComponent::setCurrentDistance, reactionTime, true);
}

void UTP_AITankBehaviorComponent::DrawDebugTank(UWorld* _world, FVector _start, FVector _end, float _progress)
{
	if (!_world)return;
	//DrawDebugCylinder(_world, _start, _end, 100, 20, FColor::Blue, false, -1, 0, 2);
	//DrawDebugCylinder(_world, _start, FVector(_end.X, _end.Y, _end.Z * _progress) , 100, 20, FColor::Red, false, -1, 0, 2);

}

void UTP_AITankBehaviorComponent::setCurrentDistance()
{
	Super::setCurrentDistance();
}

void UTP_AITankBehaviorComponent::PlasmaTimerUp()
{
	if (!target || !owner)return;
	float _distance = FVector::Distance(target->GetActorLocation(), owner->GetActorLocation());
	if (_distance <= RangePlasmaActivation || timePlasmaActivation >= tresholdPlasmaWillContinue)
	{
		if (timePlasmaActivation == 0)
			onCharge.Broadcast(false);
		onShootPlasma.Broadcast();
		onStopNormalShooting.Broadcast();
		timePlasmaActivation += worldS->DeltaTimeSeconds;
		FVector _ownerLocation = owner->GetActorLocation();
		DrawDebugTank(worldS, _ownerLocation, _ownerLocation + owner->GetActorForwardVector() * RangePlasmaActivation, timePlasmaActivation / timePlasmaActivationMax);
		float _percent = timePlasmaActivation / timePlasmaActivationMax;
		_percent = _percent > 1 ? 1 : _percent;
		plasmaCurentCharge->SetWorldScale3D(FVector(plasmaCurentCharge->DecalSize.X / 100, plasmaCurentCharge->DecalSize.Y / 50* _percent, plasmaCurentCharge->DecalSize.Z / 50 * _percent));
		if (!plasmaCurentCharge || !plasmaMaxRange)return;
		plasmaMaxRange->SetVisibility(true);
		plasmaCurentCharge->SetVisibility(true);
	}
	else if (timePlasmaActivation == 0)
	{
		onStopShootingPlasma.Broadcast();
		if (!plasmaCurentCharge || !plasmaMaxRange)return;
		plasmaMaxRange->SetVisibility(false);
		plasmaCurentCharge->SetVisibility(false);
	}
}

