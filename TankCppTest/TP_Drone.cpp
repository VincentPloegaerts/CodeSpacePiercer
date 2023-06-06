#include "TP_Drone.h"
#include "TP_Room.h"
#include "TP_AIBehaviorComponent.h"

ATP_Drone::ATP_Drone()
{
	PrimaryActorTick.bCanEverTick = true;
	aiMovement = CreateDefaultSubobject<UTP_AIMovementComponent>(TEXT("AIMovement"));
	aiShoot = CreateDefaultSubobject<UTP_ShootAIMinigunSystemComponent>(TEXT("AIShoot"));
}

void ATP_Drone::BeginPlay()
{
	Super::BeginPlay();
	Init(GetWorld());
	materialMeshDrone = GetSkeletalMesh()->CreateDynamicMaterialInstance(0, GetSkeletalMesh()->GetMaterial(0));
	if (!aiBehaviour)return;
	aiBehaviour->OnTargetLost().AddDynamic(this, &ATP_Drone::Lost);
	aiBehaviour->OnTargetReached().AddDynamic(this, &ATP_Drone::Attack);
	aiBehaviour->OnTargetNotInReachAnymore().AddDynamic(this, &ATP_Drone::Detected);

	//APawn::bUseControllerRotationYaw = 0;

	if (!aiShoot)return;
	onDroneAttack.AddDynamic(aiShoot, &UTP_ShootAIMinigunSystemComponent::ActivateShoot);
	onDroneTargetAcquired.AddDynamic(aiShoot, &UTP_ShootAIMinigunSystemComponent::DesactivateShoot);
}
void ATP_Drone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!wasBumped)return;
	onBumped.Broadcast();
}

void ATP_Drone::LifeSpanExpired()
{
	GetRoomOwner()->RemoveDrone(this);
	Destroy();
}
void ATP_Drone::Attack()
{
	onDroneAttack.Broadcast();
	if (!aiMovement)return;
	aiMovement->SetActOnGoing(1);
}
void ATP_Drone::Detected()
{
	onDroneTargetAcquired.Broadcast();
	if (!aiMovement)return;
	aiMovement->SetActOnGoing(0);
}
void ATP_Drone::Lost()
{
	onDroneTargetLost.Broadcast();
	if (!aiMovement)return;
	aiMovement->SetActOnGoing(2);
}
void ATP_Drone::ActorDeath(AActor* _deadActor)
{
	if (aiBehaviour)
	{
		aiBehaviour->OnTargetReached().RemoveDynamic(this, &ATP_Drone::Attack);
		aiBehaviour->OnTargetNotInReachAnymore().RemoveDynamic(this, &ATP_Drone::Detected);
	}
	if (aiShoot)
	{
		onDroneAttack.RemoveDynamic(aiShoot, &UTP_ShootAIMinigunSystemComponent::ActivateShoot);
		onDroneTargetAcquired.RemoveDynamic(aiShoot, &UTP_ShootAIMinigunSystemComponent::DesactivateShoot);
		aiShoot->DesactivateShoot();
	}
	Lost();
	ATP_Drone* _deadDrone = Cast<ATP_Drone>(_deadActor);
	if (!_deadDrone || !_deadDrone->GetRoomOwner())return;
	_deadDrone->GetRoomOwner()->RemoveDrone(_deadDrone);
	Super::ActorDeath(_deadActor);
}
void ATP_Drone::Init(UWorld* _world)
{
	Super::Init(_world);
	ATP_Room* _owner = GetRoomOwner();
	if (!_owner)return;
	_owner->AddToDroneArray(this);
}
void ATP_Drone::UpdateMaterial(float _health, float _maxHealth)
{
	if (!materialMeshDrone)return;
	materialMeshDrone->SetScalarParameterValue("damage_state", 1 - (_health / _maxHealth));
}