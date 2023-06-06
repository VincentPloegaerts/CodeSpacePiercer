#include "TP_NukeBullet.h"
#include "TP_GameCharacter.h"
#include "NiagaraFunctionLibrary.h"

ATP_NukeBullet::ATP_NukeBullet()
{

}
void ATP_NukeBullet::BeginPlay()
{
	Super::BeginPlay();
	projectileMovementComponent->Velocity = FVector(0,0,1)*moveSpeed;
	projectileMovementComponent->UpdateComponentVelocity();
}
void ATP_NukeBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BulletBehavior();
}

void ATP_NukeBullet::SetTarget(ATP_GameCharacter* _target)
{
	target = _target;
}

void ATP_NukeBullet::DealDammage(AActor* _me, AActor* _other)
{
	if (_other != target)return;
	ATP_GameCharacter* _actor = Cast<ATP_GameCharacter>(_other);
	_actor->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	if(niagaraOnImpact)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, niagaraOnImpact, _actor->GetActorLocation());
	Super::DealDammage(_me,_other);
}
void ATP_NukeBullet::BulletBehavior()
{
	if (!target)return;
	if (target->GetHealthComponent()->IsDead())
	{
		Destroy();
		return;
	}
	FVector _direction = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	if (!projectileMovementComponent)return;
	projectileMovementComponent->Velocity = FMath::Lerp(projectileMovementComponent->Velocity,_direction * moveSpeed,FMath::Pow(time, speedCourbe));
	projectileMovementComponent->UpdateComponentVelocity();
	time += GetWorld()->DeltaTimeSeconds;
	time = time >= 1 ? 1 : time;
}
