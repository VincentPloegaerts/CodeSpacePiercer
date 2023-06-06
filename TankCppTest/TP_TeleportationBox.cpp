#include "TP_TeleportationBox.h"
#include "TP_TankPlayer.h"
#include "Kismet/GameplayStatics.h"

ATP_TeleportationBox::ATP_TeleportationBox()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	if (triggerBox)triggerBox->SetupAttachment(RootComponent);

}

void ATP_TeleportationBox::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ATP_TeleportationBox::Teleport);
}

void ATP_TeleportationBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATP_TeleportationBox::Teleport(AActor* _mine, AActor* _other)
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(_other);
	if (!_player)return;

	_player->SetActorLocation(teleportationCoord);
	if(cancelVelocity)
		_player->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	if (setView)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(FRotator(0, teleportationZRotation, 0));

}

