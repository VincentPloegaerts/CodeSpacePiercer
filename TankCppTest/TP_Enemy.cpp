#include "TP_Enemy.h"
#include "TP_GameMode.h"
#include "TP_RoomManager.h"
#include "TP_AIBehaviorComponent.h"
#include "TP_Room.h"
#include "Components/PrimitiveComponent.h"
#include "NiagaraFunctionLibrary.h"

ATP_Enemy::ATP_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	niagaraFxMidLife = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraDamage"));
	aiBehaviour = CreateDefaultSubobject<UTP_AIBehaviorComponent>(TEXT("AiBehaviour"));
	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	sphereComponent->SetupAttachment(RootComponent);
	niagaraFxMidLife->SetupAttachment(RootComponent);
	GetCapsuleComponent()->SetCollisionProfileName("Enemy", true);
}
void ATP_Enemy::BeginPlay()
{
	Super::BeginPlay();
	niagaraFxMidLife->Deactivate();
	OnDestroyed.AddDynamic(this, &ATP_Enemy::DeathFx);
	healthComponent->OnMidLife().AddDynamic(niagaraFxMidLife,&UNiagaraComponent::Activate);
	Init(GetWorld());
}
void ATP_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ATP_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATP_Enemy::Activate()
{
	Super::Activate();
	aiBehaviour->Activation();
}
void ATP_Enemy::Desactivate()
{
	Super::Desactivate();
}
void ATP_Enemy::Init(UWorld* _world)
{
	if (!_world)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Couldn't Get World"));
		return;
	}

	ATP_GameMode* _gameMode = Cast<ATP_GameMode>(_world->GetAuthGameMode());
	if (!_gameMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Couldn't Get Game Mode, Add it and Set it in level and in world settings (in the BP)"));
		return;
	}

	TSoftObjectPtr<ATP_GameLogic> _gameLogic = _gameMode->GetGameLogic();
	if (!_gameLogic)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Couldn't Get Game Logic, Add it and Set it in level and in GameMode"));
		return;
	}

	SetRoomManager(_gameLogic->GetRoomManager());
	if (!roomManager)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Couldn't Get Room Manager, Add it and Set it in level and in GameLogic"));
		return;
	}
	roomManager->SetRoomOwnerForEnemy(roomOwnerName, this);
}
FVector ATP_Enemy::GetSphereScale()
{
	return sphereComponent->GetComponentScale();
}

void ATP_Enemy::DeathFx(AActor* _b)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, niagaraOnDeath, this->GetActorLocation(),FRotator(),FVector(1,1,1),false);
}

void ATP_Enemy::ActorDeath(AActor* _deadActor)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, enemyExplosionFx, this->GetActorLocation(), FRotator(), FVector(1, 1, 1), false);
	GetMesh()->SetCustomDepthStencilValue(0);
	GetShieldComponent()->DestroyShield();
	GetShieldComponent()->DestroyShieldActor();
	SetLifeSpan(deathAnimDuration);
	if(!deathAnimCollisionEnable)
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
