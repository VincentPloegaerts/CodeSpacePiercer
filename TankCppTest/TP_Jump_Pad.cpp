#include "TP_Jump_Pad.h"
#include "GameFramework/CharacterMovementComponent.h"

ATP_Jump_Pad::ATP_Jump_Pad()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	padMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	padFX1 = CreateDefaultSubobject<UNiagaraComponent>("padFX1");
	padFX2 = CreateDefaultSubobject<UNiagaraComponent>("padFX2");
	padFX3 = CreateDefaultSubobject<UNiagaraComponent>("padFX3");

	boxComponent->SetupAttachment(RootComponent);
	if (padMesh)padMesh->SetupAttachment(RootComponent);
	
	padFX1->SetupAttachment(padMesh);
	padFX2->SetupAttachment(padMesh);
	padFX3->SetupAttachment(padMesh);

}

void ATP_Jump_Pad::ActivateJumpPad()
{
	isActive = true;
	padFX1->SetAsset(niagaraActivation);
	padFX1->Activate(true);
	padFX2->Activate(true);
	padFX3->Activate(true);

}

void ATP_Jump_Pad::DesactivateJumpPad()
{
	isActive = false;
	padFX1->SetAsset(niagaraDesactivation);
	padFX2->Deactivate();
	padFX3->Deactivate();
}

void ATP_Jump_Pad::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ATP_Jump_Pad::Bump);
	player = Cast<ATP_TankPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!player)return;
	onJumpadActivateForPlayer.AddDynamic(player, &ATP_TankPlayer::ChangeRaycastRange);
}

void ATP_Jump_Pad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATP_Jump_Pad::Bump(AActor* _me, AActor* _other)
{
	if (!isActive)return;
	ATP_GameCharacter* _target = Cast<ATP_GameCharacter>(_other);
	if (!_target)return;
	_target->GetCharacterMovement()->Velocity.Z = 0;
	_target->GetCharacterMovement()->AddImpulse(FVector::ZAxisVector * jumpadPower);

	ATP_TankPlayer* _targetPlayer = Cast<ATP_TankPlayer>(_target);
	if (!_targetPlayer)return;
	onJumpadActivateForPlayer.Broadcast();
}

