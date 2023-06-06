#include "TP_LootPince.h"
#include "Kismet/KismetMathLibrary.h"

ATP_LootPince::ATP_LootPince()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	startPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StartPoint"));
	endPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndPoint"));
	startPoint->SetupAttachment(RootComponent);
	endPoint->SetupAttachment(RootComponent);
	pinceArm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PinceArm"));
	pinceBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PinceBox"));
	pinceArm->SetupAttachment(RootComponent);
	pinceBox->SetupAttachment(pinceArm);

	playerTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerTriggerBox"));
	playerTriggerBox->SetupAttachment(RootComponent);
	groundTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GroundTriggerBox"));
	groundTriggerBox->SetupAttachment(RootComponent);
}

void ATP_LootPince::BeginPlay()
{
	Super::BeginPlay();
	startLocation = startPoint->GetComponentLocation() + FVector(0, 0, heightOffset);
	endLocation = endPoint->GetComponentLocation() + FVector(0, 0, heightOffset);
	playerTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATP_LootPince::StartingPinceMovement);
	groundTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATP_LootPince::LootContent);
	onClawHit.AddDynamic(this, &ATP_LootPince::DropLootBox);
}

void ATP_LootPince::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeProgress();
}


void ATP_LootPince::PinceMovement()
{
	FVector _slide=UKismetMathLibrary::VLerp(startLocation, endLocation, time);
	pinceArm->SetWorldLocation(_slide);
	if (time >= 1 && canLoop)
	{
		ChangePinceTranslation();
	}
}

void ATP_LootPince::StartingPinceMovement(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	playerInRange = true;
}

void ATP_LootPince::TimeProgress()
{
	if (!playerInRange)return;
	time += GetWorld()->DeltaTimeSeconds *clawSpeed;
	time = time > 1 ? 1 : time;
	PinceMovement();
}

void ATP_LootPince::LootContent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	if (!pinceBox)return;
	onBoxHitGround.Broadcast();
	for (size_t i = 0; i < content.Num(); i++)
	{
		float _Xoffset = UKismetMathLibrary::RandomFloatInRange(-150, 150);
		float _Yoffset = UKismetMathLibrary::RandomFloatInRange(-150, 150);
		ATP_PickUp* _tempo=GetWorld()->SpawnActor<ATP_PickUp>(content[i], FTransform(pinceBox->GetComponentLocation()+FVector (_Xoffset,_Yoffset, 0)));
		_tempo->SetLifeSpan(360);
	}
	pinceBox->DestroyComponent();
	pinceBox = nullptr;
}

void ATP_LootPince::ChangePinceTranslation()
{
	time = 0;
	FVector _tempo = startLocation;
	startLocation = endLocation;
	endLocation = _tempo;
}

void ATP_LootPince::DropLootBox()
{
	if (!pinceBox)return;
	pinceBox->SetSimulatePhysics(true);
}

