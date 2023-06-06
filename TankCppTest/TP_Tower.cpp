#include "TP_Tower.h"
#include "TP_AIBehaviorComponent.h"
#include "TP_ShootTowerSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TP_Room.h"
#include "TP_RoomManager.h"

ATP_Tower::ATP_Tower()
{
	PrimaryActorTick.bCanEverTick = true;
	shootAiSystem = CreateDefaultSubobject<UTP_ShootTowerSystemComponent>("ShootAiSystem");
	towerBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerBase"));
	towerBase->SetupAttachment(RootComponent);
	towerBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	if (triggerBox)triggerBox->SetupAttachment(RootComponent);
	if (!GetMesh())return;
	GetMesh()->SetupAttachment(towerBase);
}
void ATP_Tower::BeginPlay()
{
	Super::BeginPlay();
	towerTurret = GetMesh();
	localLocation = GetActorLocation();
	if (!shieldComponent)return;
	shieldComponent->SetDamageable(false);
	if (!aiBehaviour)return;
	target = aiBehaviour->GetTarget();
	aiBehaviour->OnTargetLost().AddDynamic(this, &ATP_Tower::Lost);
	aiBehaviour->OnTargetReached().AddDynamic(this, &ATP_Tower::Attack);
	Desactivate();
	materialTowerBase = towerBase->CreateDynamicMaterialInstance(0, towerBase->GetMaterial(0));
	materialTowerTurret = towerTurret->CreateDynamicMaterialInstance(0, towerTurret->GetMaterial(0));
}
void ATP_Tower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!target || !canRotate)return;
	RotationTower(localLocation, target->GetActorLocation() + target->GetVelocity()*anticipationSpeed);
}
void ATP_Tower::Init(UWorld* _world)
{
	Super::Init(_world);
	if (!roomOwner)return;
	roomOwner->AddToTowerArray(this);
}

void ATP_Tower::RotationTower(FVector _start,FVector _target)
{
	if (!towerTurret)return;
	FRotator _rotation = UKismetMathLibrary::FindLookAtRotation(_start, _target);
	FRotator _wantedRotation = FRotator(_rotation.Pitch < minTurretInclinaison ? 0 : _rotation.Pitch > maxTurretInclinaison ? maxTurretInclinaison : _rotation.Pitch, _rotation.Yaw, 0);
	towerTurret->SetWorldRotation(FMath::Lerp(_rotation,_wantedRotation,0.2f));
}
void ATP_Tower::Lost()
{
	if (!shootAiSystem)return;
	shootAiSystem->DesactivateShoot();
	canRotate = false;
	onTargetLost.Broadcast();
}
void ATP_Tower::Attack()
{
	if (!shootAiSystem)return;
	shootAiSystem->ActivateShoot();
	canRotate = true;
	onAttack.Broadcast();
}
void ATP_Tower::ActorDeath(AActor* _deadActor)
{
	Super::ActorDeath(_deadActor);
	towerBase->SetCustomDepthStencilValue(0);
	aiBehaviour->OnTargetReached().RemoveDynamic(this, &ATP_Tower::Attack);
	Lost();
	ATP_Tower* _deadTower = Cast<ATP_Tower>(_deadActor);
	if (!_deadTower || !_deadTower->GetRoomOwner())return;
	_deadTower->GetRoomOwner()->RemoveTower(_deadTower);
}

void ATP_Tower::UpdateMaterial(float _health, float _maxHealth)
{
	if (!materialTowerBase || !materialTowerTurret)return;
	materialTowerBase->SetScalarParameterValue("damage_state", 1-( _health/_maxHealth));
	materialTowerTurret->SetScalarParameterValue("damage_state",1-( _health / _maxHealth));
}