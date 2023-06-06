#include "TP_AIMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h"
//#include "TP_TankEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UTP_AIMovementComponent::UTP_AIMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UTP_AIMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	target = Cast<ATP_TankPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	owner = Cast<ATP_GameCharacter>(GetOwner());
	if (!owner)return;
	controller = owner->GetInstigatorController();
	if (target)
	{
		target->OnPlayerFaling().AddDynamic(this, &UTP_AIMovementComponent::TargetFalling);
		target->OnNavMeshHit().AddDynamic(this, &UTP_AIMovementComponent::GetRayCastImpactLocation);
	}
}
void UTP_AIMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (isStuned)
	{
		StunBehaviour(DeltaTime);
		return;
	}
	if (!target || !owner || !owner->IsCharacterActive())return;
	switch (modeActionEnCours)
	{
	case 0:
		MoveToTarget(target);
		break;
	case 1:
		MoveAroundTarget(DeltaTime, target);
		break;
	case 2:
		StopMoving();
		break;
	}
	if (modeActionEnCours != 2)
		LookAt(target);
}

#pragma region basicMovement
void UTP_AIMovementComponent::MoveToTarget(ATP_TankPlayer* _target)
{
	if (!_target)return;
	if(!updateTargetPos && owner->GetVelocity() == FVector::ZeroVector)
	{
		if (!isBeginPosAttributed)
		{
			beginPos = owner->GetActorLocation();
			isBeginPosAttributed = true;
		}
		MoveToJump();
	}
	else
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, targetPos);
}
void UTP_AIMovementComponent::MoveAroundTarget(float DeltaTime, ATP_TankPlayer* _target)
{

	if (!target)return;
	FVector _ownerLoc = owner->GetActorLocation();
	ResetStats();

#pragma region ScaryMath
	currentRotationAngle += rotationAngle * DeltaTime * rotationSpeed;
	currentRotationAngle = FMath::Fmod(currentRotationAngle, 360);
	const float& _toRad = FMath::DegreesToRadians(currentRotationAngle);
	const float& _x = FMath::Cos(_toRad) * rotationDistance;
	const float& _y = FMath::Sin(_toRad) * rotationDistance;
	const FVector& _location = FVector(_x, _y, 0);
	const FVector& _finalLocation = (target->GetActorLocation() + _location);
	FVector _testinMoveDist = _ownerLoc - _finalLocation;
#pragma endregion

	//UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetOwner()->GetInstigatorController(), _finalLocation);
	UCharacterMovementComponent* _ownerCharacMove = owner->GetCharacterMovement();
	_ownerCharacMove->Velocity.X = (_finalLocation - _ownerLoc).X;
	_ownerCharacMove->Velocity.Y = (_finalLocation - _ownerLoc).Y;
}
void UTP_AIMovementComponent::StopMoving()
{
	UAIBlueprintHelperLibrary::SimpleMoveToActor(controller, owner);
	ResetStats();
}
#pragma endregion
void UTP_AIMovementComponent::LookAt(ATP_TankPlayer* _target)
{
	if (!_target)return;
	
	if (modeActionEnCours == 1)
	{
		//look at player if is attacking
		FVector _playerLoc = _target->GetActorLocation();
		FVector _ownerLoc = owner->GetActorLocation();
		FVector _direction = (_playerLoc - _ownerLoc).GetSafeNormal();
		_direction.Z = 0;

		owner->SetActorRotation(_direction.ToOrientationRotator());
	}
	else if (isBeginPosAttributed)
	{
		FVector _look = lastAerialPosOffTarget - beginPos;
		_look.Z = 0;
		FRotator _toLook = _look.ToOrientationRotator();
		owner->SetActorRotation((_toLook));
	}
}
void UTP_AIMovementComponent::SetActOnGoing(int _index)
{
	if(_index>-1 && _index<3)
		modeActionEnCours = _index;
	if (_index == 1)
	{
		if (!target)return;
		rotationDistance = FVector::Distance(owner->GetActorLocation(), target->GetActorLocation());
		currentRotationAngle = owner->GetActorRotation().Yaw + 180;
	}
}
#pragma region SendFromPlayer
/// <summary>
/// Check if the targeted player jumped/fall from a platform
/// </summary>
void UTP_AIMovementComponent::TargetFalling()
{
	//if (isTank) return;
	if (updateTargetPos == false)return;
	updateTargetPos = false;

	FTimerHandle _timerCheckDistance;
	GetWorld()->GetTimerManager().SetTimer(_timerCheckDistance, this, &UTP_AIMovementComponent::GetLastAerialPosOffTarget, 0.1, false);
}
void UTP_AIMovementComponent::GetRayCastImpactLocation(FVector _impactPos)
{
	if (updateTargetPos)
		targetPos = _impactPos;
	else
	{
		toCompare = _impactPos;
		PathComparator();
	}
}
#pragma endregion

#pragma region Stun
void UTP_AIMovementComponent::StunBehaviour(float _DeltaTime)
{
	
	timerStuned -= _DeltaTime;

	if (timerStuned > 0)return;
	isStuned = false;
	onDroneUnStuned.Broadcast();
}
void UTP_AIMovementComponent::Stunned(float _timeStuned)
{
	if (!isStunable)return;
	timerStuned = _timeStuned;
	isStuned = true;
	onDroneStuned.Broadcast();
	StopMoving();
}
#pragma endregion
#pragma region delayedFct
void UTP_AIMovementComponent::GetLastAerialPosOffTarget()
{
	lastAerialPosOffTarget = target->GetActorLocation();
	lastAerialPosOffTarget.Z += 100;
}
void UTP_AIMovementComponent::ResetStats()
{
	updateTargetPos = true;
	isBeginPosAttributed = false;
}
#pragma endregion
void UTP_AIMovementComponent::MoveToJump()
{
	FVector _toMove = (lastAerialPosOffTarget - beginPos) * jumpStrength;
	float _toMoveSize = _toMove.Size();
	_toMove = _toMoveSize > maxJumpDist ? _toMove / (_toMoveSize / maxJumpDist) : _toMove;
	_toMove = _toMoveSize < minJumpDist ? _toMove / (_toMoveSize / minJumpDist) : _toMove;
	owner->GetCharacterMovement()->AddImpulse(_toMove);

	FTimerHandle _timerCheckDistance;
	GetWorld()->GetTimerManager().SetTimer(_timerCheckDistance, this, &UTP_AIMovementComponent::ResetStats, 0.3f, false);
}
void UTP_AIMovementComponent::PathComparator()
{
	UNavigationSystemV1* _navSys = controller ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(controller->GetWorld()) : nullptr;
	if (!_navSys)return;
	const FVector _agentNavLocation = controller->GetNavAgentLocation();
	const ANavigationData* _navData = _navSys->GetNavDataForProps(controller->GetNavAgentPropertiesRef(), _agentNavLocation);
	if (_navData)
	{
		FPathFindingQuery _query(controller, *_navData, _agentNavLocation, targetPos);
		FPathFindingResult _result = _navSys->FindPathSync(_query);
		float _currentLength = _result.Path.Get()->GetLength();

		FPathFindingQuery _queryBis(controller, *_navData, _agentNavLocation, toCompare);
		FPathFindingResult _resultBis = _navSys->FindPathSync(_queryBis);
		float _other = _resultBis.Path.Get()->GetLength();

		if (_other < _currentLength)
			targetPos = toCompare;
	}
	
}
