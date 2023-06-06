#include "TP_TankEnemy.h"
#include "TP_ShootPlasmaComponent.h"
#include "TP_ShootMinigunSystemComponent.h"
#include "TP_Room.h"
#include "TP_AITankBehaviorComponent.h"
#include "TP_HUD.h"
#include "NiagaraFunctionLibrary.h"


ATP_TankEnemy::ATP_TankEnemy()
{
	plasmaComponent = CreateDefaultSubobject<UTP_ShootPlasmaComponent>(TEXT("PlasmaComponent"));
	aiBehaviour->MarkPendingKill();
	aiBehaviour = CreateDefaultSubobject<UTP_AITankBehaviorComponent>(TEXT("AITankComponent"));
	meshTurret = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshTurret"));
	meshRadar = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshRadar"));
	plasmaLoading = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LoadingPlasmaFx"));
	plasmaLoading->SetupAttachment(RootComponent);
	aiMovement = CreateDefaultSubobject<UTP_AITankMovementComponent>(TEXT("AIMovement"));
	aiMinigunShootComponent = CreateDefaultSubobject<UTP_ShootAIMinigunSystemComponent>(TEXT("aiMinigunShoot"));
	meshTurret->SetupAttachment(GetMesh());
	meshRadar->SetupAttachment(GetMesh());
	plasmaCurentCharge = CreateDefaultSubobject<UDecalComponent>("plasma CurentCharge");
	plasmaMaxRange = CreateDefaultSubobject<UDecalComponent>("plasma Max Range");
}
void ATP_TankEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (!shieldComponent)return;
	shieldComponent->SetDamageable(false);
	Init(GetWorld());
	healthComponent->OnMidLife().AddDynamic(this, &ATP_TankEnemy::RestoreShield);
	if (!aiBehaviour)return;
	aiBehaviour->OnTargetLost().AddDynamic(this, &ATP_TankEnemy::Lost);
	aiBehaviour->OnTargetReached().AddDynamic(this, &ATP_TankEnemy::Attack);
	aiBehaviour->OnTargetNotInReachAnymore().AddDynamic(this, &ATP_TankEnemy::Detected);
	ATP_HUD* _tempoHud = Cast<ATP_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!_tempoHud)return;
	dialogueUI = _tempoHud->GetDialogueUI();
	materialBaseTank = GetMesh()->CreateDynamicMaterialInstance(0, GetMesh()->GetMaterial(0));
	materialTurretTank = meshTurret->CreateDynamicMaterialInstance(0, meshTurret->GetMaterial(0));
	materialRadarTank = meshRadar->CreateDynamicMaterialInstance(0, meshRadar->GetMaterial(0));
	UTP_AITankBehaviorComponent* _temp = Cast<UTP_AITankBehaviorComponent>(aiBehaviour);
	if (!plasmaLoading || !aiBehaviour)return;
	plasmaLoading->Deactivate();
	_temp->OnCharge().AddDynamic(plasmaLoading, &UNiagaraComponent::Activate);
	_temp->OnStopShootingPlasma().AddDynamic(plasmaLoading, &UNiagaraComponent::Deactivate);
}
void ATP_TankEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(doRadarMeshRotate)
		meshRadar->AddLocalRotation(FRotator(0, 1, 0));
}

void ATP_TankEnemy::LifeSpanExpired()
{
	if (!GetRoomOwner())return;
	GetRoomOwner()->RemoveTank(this);
	if (!this)return;
	Destroy();
}
void ATP_TankEnemy::Init(UWorld* _world)
{
	Super::Init(_world);
	ATP_Room* _owner = GetRoomOwner();
	if (!_owner)return;
	_owner->AddToTankArray(this);
}
void ATP_TankEnemy::Attack()
{
	//aiMinigunShootComponent->ActivateShoot();
}
void ATP_TankEnemy::Lost()
{
	//aiMinigunShootComponent->DesactivateShoot();
	if (!aiMovement)return;
	UTP_AITankBehaviorComponent* _tempo = Cast<UTP_AITankBehaviorComponent>(aiBehaviour);
	_tempo->OnStopNormalShooting().Broadcast();
	onTankEnnemyTargetLost.Broadcast();
	aiMovement->SetActOnGoing(2);
}
void ATP_TankEnemy::Detected()
{
	onTankEnnemyTargetAcquired.Broadcast();
	if (!aiMovement)return;
	aiMovement->SetActOnGoing(0);
}
void ATP_TankEnemy::RestoreShield(bool _isMidLife)
{
	if (shieldWasRestored)return;
	if(!_isMidLife)return;
	shieldWasRestored = true;
	shieldComponent->AddShield(shieldComponent->maxShield);
	onHalfLife.Broadcast();
}
void ATP_TankEnemy::DeathText()
{
	if (!dialogueUI)
	{
		ATP_HUD* _hud = Cast<ATP_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (!_hud)return;
		dialogueUI = _hud->GetDialogueUI();
	}

	int _len = deathText.Num();
	if (_len == 0)return;
	int _rand = FMath::RandRange(0, _len-1);
	if (!dialogueUI)return;
	dialogueUI->SetSpeechText(deathText[_rand]);
}

void ATP_TankEnemy::ActorDeath(AActor* _deadActor)
{
	Super::ActorDeath(_deadActor);
	meshTurret->SetCustomDepthStencilValue(0);
	meshRadar->SetCustomDepthStencilValue(0);
	aiBehaviour->OnTargetLost().RemoveDynamic(this, &ATP_TankEnemy::Lost);
	aiBehaviour->OnTargetReached().RemoveDynamic(this, &ATP_TankEnemy::Attack);
	aiBehaviour->OnTargetNotInReachAnymore().RemoveDynamic(this, &ATP_TankEnemy::Detected);
	Cast<UTP_AITankBehaviorComponent>(aiBehaviour)->SetDead();
	Lost();
	ATP_TankEnemy* _deadTankEnnemy = Cast<ATP_TankEnemy>(_deadActor);
	if (!_deadTankEnnemy || !_deadTankEnnemy->GetRoomOwner())return;
	_deadTankEnnemy->GetRoomOwner()->RemoveTank(_deadTankEnnemy);
	DeathText();
}

void ATP_TankEnemy::UpdateMaterial(float _health, float _maxHealth)
{
	if (!materialBaseTank || !materialTurretTank || !materialRadarTank)return;
	materialBaseTank->SetScalarParameterValue("damage_state", 1 - (_health / _maxHealth));
	materialTurretTank->SetScalarParameterValue("damage_state", 1 - (_health / _maxHealth));
	materialRadarTank->SetScalarParameterValue("damage_state", 1 - (_health / _maxHealth));
}