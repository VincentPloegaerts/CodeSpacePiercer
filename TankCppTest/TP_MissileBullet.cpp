#include "TP_MissileBullet.h"
#include "TP_TankPlayer.h"
#include "TP_Drone.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialLayersFunctions.h"

ATP_MissileBullet::ATP_MissileBullet()
{
	targetedZone = CreateDefaultSubobject<UDecalComponent>("Targeted Zone");
	targetedMaxSizeZone = CreateDefaultSubobject<UDecalComponent>("Targeted Zone Max Size");

	explosionFX = CreateDefaultSubobject<UNiagaraComponent>("EffectExplosion");
	if (!explosionFX)return;
	explosionFX->SetupAttachment(RootComponent);

}

void ATP_MissileBullet::BeginPlay()
{
	Super::BeginPlay();
	if (!explosionFX)return;
	OnActorHit.RemoveDynamic(this, &ATP_MissileBullet::OnHit);
	//explosionFX->SetTickableWhenPaused(false);
	//explosionFX->SetPaused(true);
	isAOE = true;
	projectileMovementComponent->Velocity = FVector(0, 0, 1) * moveSpeed;
	projectileMovementComponent->UpdateComponentVelocity();

	materialInstance = staticMesh->CreateDynamicMaterialInstance(0);
	materialInstance->SetVectorParameterValue(colorParamName, BaseColorGrowing);

	/*
	FHashedMaterialParameterInfo _test;
	_test.Name.ToString() = "BaseColor";
	targetedZone->GetDecalMaterial()->GetMaterial()->OverrideVectorParameterDefault(_test, BaseColorGrowing, true,ERHIFeatureLevel::SM5);
	targetedMaxSizeZone->GetDecalMaterial()->GetMaterial()->OverrideVectorParameterDefault(_test, BaseColorGrowing, true, ERHIFeatureLevel::SM5);
	*/

	//targetedMaxSizeZone->GetDecalMaterial()->GetMaterial()->BaseColor = BaseColorExternal;
	SetLifeSpan(10);
	SetTarget();
}

void ATP_MissileBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BulletBehavior();
}

void ATP_MissileBullet::SetTarget()
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	TArray<AActor*> _toIgnor;
	FHitResult _target;
	if (!_player)return;
	_toIgnor.AddUnique(this);
	_toIgnor.AddUnique(_player);
	target = _player->GetActorLocation();
	target.X += FMath::RandRange(-randomAroundPlayer, randomAroundPlayer);
	target.Y += FMath::RandRange(-randomAroundPlayer, randomAroundPlayer);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(),
											target,
											target + FVector::ZAxisVector * -10000,
											ETraceTypeQuery::TraceTypeQuery1,
											false,
											_toIgnor,
											EDrawDebugTrace::None,
											_target,
											true,
											FLinearColor::Red,
											FLinearColor::Green,
											1);

	targetGND = _target.ImpactPoint;


	dist = FVector::Distance(targetGND, GetActorLocation());
	targetedZone->DecalSize.X = decalComponentHightHit;
	size = targetedZone->DecalSize;
	targetedMaxSizeZone->SetWorldScale3D(FVector(size.X, size.Y * (1.f / impactSizeDivider), size.Z * (1.f / impactSizeDivider)));
	AOERange = size.Y*(250/ impactSizeDivider);

	CalculateDecalRot();

}

void ATP_MissileBullet::BulletBehavior()
{
	if (asHit)
	{
		projectileMovementComponent->Velocity = FVector(0, 0, 0);
		return;
	}
	FVector _direction = (target - GetActorLocation()).GetSafeNormal();
	float _dist = FVector::Distance(target, GetActorLocation());

	if(time* growSpeed <1)
		targetedZone->SetWorldScale3D(FVector(size.X, size.Y * ((time* growSpeed) / impactSizeDivider), size.Z * ((time* growSpeed) / impactSizeDivider)));
	else
		targetedZone->SetWorldScale3D(FVector(size.X, size.Y * (1.f / impactSizeDivider), size.Z * (1.f / impactSizeDivider)));

	if (!projectileMovementComponent)return;
	projectileMovementComponent->Velocity = FMath::Lerp(projectileMovementComponent->Velocity, _direction * moveSpeed, FMath::Pow(time, speedCourbe));
	projectileMovementComponent->UpdateComponentVelocity();
	FRotator _toRotate = projectileMovementComponent->Velocity.ToOrientationRotator();
	_toRotate.Pitch += 90;
	SetActorRotation(_toRotate);

	time += GetWorld()->DeltaTimeSeconds;
	time = time >= 1 ? 1 : time;

	if (time > 0.5)
		target = targetGND;

}

void ATP_MissileBullet::DealDammage(AActor* _me, AActor* _other)
{

	if (!goThroughWall)
		posToHit = _me->GetActorLocation();
	else
	{
		posToHit = targetGND;
		if (time < 1)return;
	}

	explosionFX->Activate();
	explosionFX->SetWorldRotation(targetImpactResult.ImpactNormal.ToOrientationRotator());
	targetedZone->SetVisibility(false);
	targetedMaxSizeZone->SetVisibility(false);
	staticMesh->SetVisibility(false);
	effectBullet->SetVisibility(false);

	asHit = true;
	if (isAOE)
	{
		bool _playerHitten = false;
		TArray<AActor*> _toIgnior;
		TArray<FHitResult> _hitResult;
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),
												posToHit,
												posToHit,
												AOERange,
												collisionSet,
												false,
												_toIgnior,
												EDrawDebugTrace::None,
												_hitResult,
												true);

		if (_hitResult.Num() > 0)
		{
			ATP_Drone* _drone;
			ATP_TankPlayer* _player;
			for (int i = 0; i < _hitResult.Num(); i++)
			{
				_drone = Cast<ATP_Drone>(_hitResult[i].Actor);
				
				if (_drone)
					_drone->GetAiMovement()->Stunned(stunedTimerDrone);
				else if(!_playerHitten)
				{
					
					_player = Cast<ATP_TankPlayer>(_hitResult[i].Actor);
					if (!_player)continue;
					_playerHitten = true;
					if (!_player->GetShieldComponent()->IsBroke())
						_player->GetShieldComponent()->TakeDamage(healthDammage);
					else
						_player->GetHealthComponent()->TakeDamage(healthDammage);

				}
			}
		}
	}
	SetLifeSpan(dispawnTimer);
}

void ATP_MissileBullet::CalculateDecalRot()
{
	FVector _direction = (target - GetActorLocation()).GetSafeNormal();
	float _dist = FVector::Dist(target, GetActorLocation());
	TArray<AActor*> _toIgnor;

	_toIgnor.AddUnique(this);

	FVector _start = FMath::Lerp(projectileMovementComponent->Velocity, _direction * moveSpeed, FMath::Pow(0.7, speedCourbe));
	float _divider = speedCourbe > 10 ? 10 : speedCourbe;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(),
											FVector((target - _start/((0.6250* moveSpeed)/ _dist)).X, (target - _start/ ((0.6250 * moveSpeed) / _dist)).Y, (GetActorLocation() + _start/ (_divider * (moveSpeed/1000))).Z), //coller > X, Y /10 // short:mid range >X, Y,Z /2  // long range Z / 3
											target,
											ETraceTypeQuery::TraceTypeQuery1,
											false,
											_toIgnor,
											EDrawDebugTrace::None,
											targetImpactResult,
											true,
											FLinearColor::Red,
											FLinearColor::Green,
											1);
	if (!targetImpactResult.bBlockingHit || Cast<ATP_TankPlayer>(targetImpactResult.GetActor()))
	{
		targetedZone->SetWorldLocation(targetGND);
		targetedMaxSizeZone->SetWorldLocation(targetGND);

	}
	else
	{
		targetedZone->SetWorldLocation(targetImpactResult.ImpactPoint);
		targetedMaxSizeZone->SetWorldLocation(targetImpactResult.ImpactPoint);
		FRotator _toRotate = targetImpactResult.ImpactNormal.ToOrientationRotator();
		targetedZone->SetWorldRotation(_toRotate);
		targetedMaxSizeZone->SetWorldRotation(_toRotate);

	}
}

