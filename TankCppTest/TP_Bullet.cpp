#include "TP_Bullet.h"
#include "TP_GameCharacter.h"
#include "TP_TankPlayer.h"
#include "TP_TankEnemy.h"
#include "TP_Enemy.h"
#include "DrawDebugHelpers.h"
#include "TP_ShieldSystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TP_Drone.h"

ATP_Bullet::ATP_Bullet()
{
	PrimaryActorTick.bCanEverTick = true;
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule Component");
	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	effectBullet = CreateDefaultSubobject<UNiagaraComponent>("EffectParticule");
	RootComponent = capsuleComponent;
	staticMesh->SetupAttachment(RootComponent);
	effectBullet->SetupAttachment(RootComponent);
	capsuleComponent->SetCollisionProfileName("Bullet", true);
}

void ATP_Bullet::BeginPlay()
{
	Super::BeginPlay();
	projectileMovementComponent->InitialSpeed = moveSpeed;
	projectileMovementComponent->ProjectileGravityScale = gravityScale;
	SetLifeSpan(distanceTraveled / moveSpeed);
	OnActorBeginOverlap.AddDynamic(this, &ATP_Bullet::DealDammage);
	OnActorHit.AddDynamic(this, &ATP_Bullet::OnHit);
}
void ATP_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATP_Bullet::DealDammage(AActor* _me, AActor* _other)
{

	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(_other);
	if (_player)
	{
		FVector _impulse = FVector(projectileMovementComponent->Velocity.X * impulsePower, projectileMovementComponent->Velocity.Y * impulsePower, projectileMovementComponent->Velocity.Z * impulsePower + upImpulse);
		if (addImpulse)
		{
			_player->GetCharacterMovement()->AddImpulse(_impulse, true);

		}
	}

	ATP_GameCharacter* _enemy = Cast<ATP_GameCharacter>(_other);
	if (!_enemy)return;

	for (size_t i = 0; i < channelCollision.Num(); i++)
		if (channelCollision[i] != _enemy->GetCapsuleComponent()->GetCollisionObjectType())return;

	ATP_Drone* _drone = Cast<ATP_Drone>(_enemy);
	if (_drone)
	{
		FVector _impulse = FVector(projectileMovementComponent->Velocity.X * impulsePower, projectileMovementComponent->Velocity.Y * impulsePower, projectileMovementComponent->Velocity.Z * impulsePower + upImpulse);
		if (addImpulse)
			_drone->GetCharacterMovement()->AddImpulse(_impulse, true);
		if (canStun)
			_drone->GetAiMovement()->Stunned(timeStun);
	}

	UTP_ShieldSystemComponent* _shield = _enemy->GetShieldComponent();
	if (_shield && !_shield->IsBroke())
	{
		if (breakShield)
			_shield->TakeDamage(-1);
		else
			_shield->TakeDamage(shieldDammage);

		if (destroyOnImpact)Destroy();
		return;
	}
	UTP_HealthComponent* _health = _enemy->GetHealthComponent();
	if (!_health)return;
	_health->TakeDamage(healthDammage);
	if (_shield)
		_shield->ResetShieldRegen();
	if (destroyOnImpact)Destroy();
}

void ATP_Bullet::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	ATP_GameCharacter* _enemy = Cast<ATP_GameCharacter>(OtherActor);
	if (!_enemy)
		if (destroyOnImpact)
			Destroy();
}

void ATP_Bullet::BulletBehavior()
{

}