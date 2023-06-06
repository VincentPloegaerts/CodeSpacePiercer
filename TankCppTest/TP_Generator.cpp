#include "TP_Generator.h"
#include "TP_Room.h"
#include "TP_TankEnemy.h"
#include "TP_AIBehaviorComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "TP_HUD.h"


ATP_Generator::ATP_Generator()
{
	positionSpawnTank = CreateDefaultSubobject<USceneComponent>("PositionSpawnTank");
	positionSpawnTank->SetupAttachment(RootComponent);
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	staticMesh->SetupAttachment(RootComponent);
}
void ATP_Generator::BeginPlay()
{
	Super::BeginPlay();
	canBeTarget = false;
	if (GetCharacterMovement())
		GetCharacterMovement()->GravityScale = 0;
	if (shieldComponent)
		shieldComponent->SetDamageable(false);
	material = staticMesh->CreateDynamicMaterialInstance(0,staticMesh->GetMaterial(0));
	EmissiveInitialValue = material->K2_GetScalarParameterValue("emissive_intensity");

	dialogueUI = Cast<ATP_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->GetDialogueUI();
}
void ATP_Generator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(shieldComponent && shieldComponent->IsDamageable() && !generatorWithProtection)
		CoolDownRestoreShield(DeltaTime);
	if (!emissiveLerping)return;
	emissiveLerpingTime += DeltaTime;
	EmissiveBehaviour();
}

void ATP_Generator::Activate()
{
	Super::Activate();
	if (shieldComponent)
		shieldComponent->SetDamageable(false);
	canBeTarget = false;
}
void ATP_Generator::SetEmissiveLerping(bool _status)
{
	emissiveLerping = _status; 
}
void ATP_Generator::RestoreShield()
{
	GetWorld()->GetTimerManager().SetTimer(spawnTankTimer,this,&ATP_Generator::SpawnTank, timeSpawnTank,false);
	shieldComponent->SetDamageable(false);
	shieldComponent->AddShield(shieldComponent->GetMaxShield());
	canBeTarget = false;
	time = 0;
}
void ATP_Generator::SpawnTank()
{
	if (!tankType)return;
	for (size_t i = 0; i < amountSpawnEnemy; i++)
	{
		ATP_TankEnemy* tankEnemy = GetWorld()->SpawnActor<ATP_TankEnemy>(tankType, positionSpawnTank->GetComponentTransform());
		if (!tankEnemy)return;
		tankEnemy->Activate();
		tankEnemy->SetRoomOwnerName(roomOwnerName);
		tankEnemy->SetRoomManager(roomManager);
		if (!roomOwner)return;
		tankEnemy->SetRoomOwner(roomOwner);
		roomOwner->AddToTankArray(tankEnemy);
		SpawnTankText();
	}
	
}

void ATP_Generator::DefenderDestroy()
{
	if (!roomOwner)return;
	if (generatorWithProtection && AreProtectorsDead())
	{
		shieldComponent->SetDamageable(true);
		shieldComponent->TakeDamage(shieldComponent->GetMaxShield());
		time = 0;
		return;
	}
	if ( !generatorWithProtection && roomOwner->LeftOnlySpawnerDronesAndGenerator() && !shieldComponent->IsDamageable())
	{
		shieldComponent->SetDamageable(true);
		shieldComponent->TakeDamage(shieldComponent->GetMaxShield());
		time = 0;
	}
}


void ATP_Generator::Init(UWorld* _world)
{
	Super::Init(_world);
	if (!GetRoomManager())return;
	if (!roomOwner)return;
	roomOwner->AddToGeneratorArray(this);
}

void ATP_Generator::ActorDeath(AActor* _deadActor)
{
	if (isDead)return;
	staticMesh->SetCustomDepthStencilValue(0);
	Super::ActorDeath(_deadActor);
	isDead = true;
	staticMesh->SetCustomDepthStencilValue(0);
	ATP_Generator* _deadGenerator = Cast<ATP_Generator>(_deadActor);
	if (!_deadGenerator || !_deadGenerator->GetRoomOwner())return;
	_deadGenerator->GetRoomOwner()->RemoveGenerator(_deadGenerator);
	UWorld* _world = GetWorld();
	if (!_world)return;
	_world->GetTimerManager().SetTimer(meshTimer,this,&ATP_Generator::ChangeMesh, timeSwitchMesh,false,-1);

	FTimerDelegate _timerDelExplo;
	FTimerDelegate _timerDelExploStop;
	_timerDelExplo.BindUFunction(this, FName("GenerateExplosionFX"), _world);
	_timerDelExploStop.BindUFunction(this, FName("StopExplosionFX"), _world);
	_world->GetTimerManager().SetTimer(explosionTimer, _timerDelExplo, explosionFXRate, true, -1);
	_world->GetTimerManager().SetTimer(explosionStopTimer, _timerDelExploStop, explosionFXDuration, false, -1);

	FTimerDelegate _timerDelLerp;
	_timerDelLerp.BindUFunction(this, FName("SetEmissiveLerping"), true);
	_world->GetTimerManager().SetTimer(lerpTimer, _timerDelLerp, timeBeforeEmissive, false, -1);
	
	
}

 void ATP_Generator::ChangeMesh()
{
	 onLastExplosion.Broadcast();
	 UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), lastExplosionFX, GetActorLocation(), FRotator::ZeroRotator, (FVector)explosionFXScale);
	 if (!meshAfterExplosion)return;
	staticMesh->SetStaticMesh(meshAfterExplosion);
	staticMesh->SetMaterial(0, materialAfterExplosion);
}

 void ATP_Generator::DestroyText()
 {
	 if (!dialogueUI)
	 {
		 ATP_HUD* _hud = Cast<ATP_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		 if (!_hud)return;
		 dialogueUI = _hud->GetDialogueUI();
	 }

	 int _len = destroyText.Num();
	 if (_len == 0)return;
	 int _rand = FMath::RandRange(0, _len-1);
	 if (!dialogueUI)return;
	 dialogueUI->SetSpeechText(destroyText[_rand]);
 }

 void ATP_Generator::SpawnTankText()
 {
	 if (!dialogueUI)
	 {
		 ATP_HUD* _hud = Cast<ATP_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		 if (!_hud)return;
		 dialogueUI = _hud->GetDialogueUI();
	 }

	 int _len = spawnTankText.Num();
	 if (_len == 0)return;
	 int _rand = FMath::RandRange(0, _len-1);
	 if (!dialogueUI)return;
	 dialogueUI->SetSpeechText(spawnTankText[_rand]);
 }

void ATP_Generator::GenerateExplosionFX(UWorld* _world)
 {
	FVector _spawnLocation = GetActorLocation() + (FMath::VRand() * explosionRange);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(_world, explosionFX, _spawnLocation, FRotator::ZeroRotator, (FVector)explosionFXScale);
	onLittleExplosion.Broadcast();

 }

void ATP_Generator::StopExplosionFX(UWorld* _world)
 {
	_world->GetTimerManager().ClearTimer(explosionTimer);
 }

void ATP_Generator::EmissiveBehaviour()
{
	if (emissiveLerpingTime < EmissiveLerpUpDuration)
		LerpValue = FMath::Lerp(EmissiveInitialValue, EmissiveMaxValue, emissiveLerpingTime / EmissiveLerpUpDuration);
	else if (emissiveLerpingTime >= EmissiveLerpUpDuration && emissiveLerpingTime < EmissiveLerpUpDuration + EmissiveLerpDownDuration)
		LerpValue = FMath::Lerp(EmissiveMaxValue, EmissiveInitialValue, (emissiveLerpingTime - EmissiveLerpUpDuration) / EmissiveLerpDownDuration);
	else if (emissiveLerpingTime >= EmissiveLerpUpDuration && emissiveLerpingTime > EmissiveLerpUpDuration + EmissiveLerpDownDuration)
		emissiveLerping = false;
	material->SetScalarParameterValue("emissive_intensity", LerpValue);
}

