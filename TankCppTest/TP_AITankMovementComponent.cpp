#include "TP_AITankMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "TP_TankEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UTP_AITankMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	if (owner)
		isTank = Cast<ATP_TankEnemy>(owner);
	if (isTank)skeletalTurretMesh = isTank->GetMeshTurret();

}

void UTP_AITankMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTP_AITankMovementComponent::MoveToTarget(ATP_TankPlayer* _target)
{
	if (!_target)return;
	if (isTank && skeletalTurretMesh)
	{
		UNavigationSystemV1* _navSys = controller ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(controller->GetWorld()) : nullptr;
		if (!_navSys)return;
		const FVector _agentNavLocation = controller->GetNavAgentLocation();
		const ANavigationData* _navData = _navSys->GetNavDataForProps(controller->GetNavAgentPropertiesRef(), _agentNavLocation);
		if (_navData)
		{
			FPathFindingQuery _query(controller, *_navData, _agentNavLocation, targetPos);
			FPathFindingResult _result = _navSys->FindPathSync(_query);
			TArray<FNavPathPoint> _path = _result.Path.Get()->GetPathPoints();
			if (_path.Num() >= 2)
			{
				FRotator _tempo = UKismetMathLibrary::FindLookAtRotation(isTank->GetActorLocation(), _path[1].Location);
				_tempo.Roll = _tempo.Pitch = 0;
				FRotator _bodyRot = UKismetMathLibrary::RLerp(isTank->GetActorRotation(), _tempo, rotationLerpValueBody, true);
				isTank->SetActorRotation(_bodyRot);
			}
			else
			{
				FRotator _tempo = UKismetMathLibrary::FindLookAtRotation(isTank->GetActorLocation(), targetPos);
				_tempo.Roll = _tempo.Pitch = 0;
				FRotator _bodyRot = UKismetMathLibrary::RLerp(isTank->GetActorRotation(), _tempo, rotationLerpValueBody / 10, true);
				isTank->SetActorRotation(_bodyRot);
			}

			FRotator _actualRota = skeletalTurretMesh->GetComponentRotation();
#pragma region yawTurretRotation
			FRotator _tempoTurretYaw = UKismetMathLibrary::FindLookAtRotation(isTank->GetActorLocation(), target->GetActorLocation());
			_tempoTurretYaw.Roll = _tempoTurretYaw.Pitch = 0;

			FRotator _turretRotYaw = UKismetMathLibrary::RLerp(isTank->GetActorRotation() + skeletalTurretMesh->GetRelativeRotation(), _tempoTurretYaw, rotationLerpValueTurret, true);

			if ((_turretRotYaw.Yaw - isTank->GetActorRotation().Yaw <= turretRotationCapYaw && _turretRotYaw.Yaw - isTank->GetActorRotation().Yaw >= -turretRotationCapYaw) || canTurretMakeFullRotationYaw)
				skeletalTurretMesh->SetWorldRotation(FRotator(_actualRota.Pitch, _turretRotYaw.Yaw, _actualRota.Roll));
#pragma endregion
			_actualRota = skeletalTurretMesh->GetComponentRotation();
#pragma region PitchTurretRotation
			FRotator _tempoTurretPitch = UKismetMathLibrary::FindLookAtRotation(isTank->GetActorLocation(), target->GetActorLocation());
			_tempoTurretPitch.Roll = _tempoTurretPitch.Yaw = 0;

			FRotator _turretRotPitch = UKismetMathLibrary::RLerp(isTank->GetActorRotation() + skeletalTurretMesh->GetRelativeRotation(), _tempoTurretPitch, rotationLerpValueTurret, true);

			if ((_turretRotPitch.Pitch - isTank->GetActorRotation().Pitch <= turretRotationCapPitch && _turretRotPitch.Pitch - isTank->GetActorRotation().Pitch >= -turretRotationCapPitch) || canTurretMakeFullRotationPitch)
				skeletalTurretMesh->SetWorldRotation(FRotator(_turretRotPitch.Pitch, _actualRota.Yaw, _actualRota.Roll));

#pragma endregion

			isTank->AddMovementInput(isTank->GetActorForwardVector() * forwardSpeedMultiplier);
		}

	}
	else
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, targetPos);
	}
}

void UTP_AITankMovementComponent::TargetFalling()
{
	if (isTank) return;

}