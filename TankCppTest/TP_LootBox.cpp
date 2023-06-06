#include "TP_LootBox.h"
#include "Kismet/KismetMathLibrary.h"

ATP_LootBox::ATP_LootBox()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseStaticMesh"));
	baseMesh->SetupAttachment(RootComponent);
	topMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopStaticMesh"));
	topMesh->SetupAttachment(baseMesh);
}

void ATP_LootBox::BeginPlay()
{
	Super::BeginPlay();
	UTP_TankCppTestGameInstance* _instance = Cast<UTP_TankCppTestGameInstance>(GetGameInstance());
	if (_instance->GetDifficulty() > difficulty)
	{
		Destroy();
		return;
	}
	if (isRandom)
	{
		onBoxDestroyed.AddDynamic(this, &ATP_LootBox::SpawnLootRandom);
		return;
	}
	onBoxDestroyed.AddDynamic(this, &ATP_LootBox::SpawnAllLoot);
}

void ATP_LootBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


//spawn tout le loot sur des coord au allentout proche de la box 
void ATP_LootBox::SpawnLootRandom()
{
	//UTP_TankCppTestGameInstance* _instance = Cast<UTP_TankCppTestGameInstance>(GetGameInstance());
	//if (_instance->GetDifficulty() > difficulty)return;
	for (size_t i = 0; i < lootNumber; i++)
	{
		int _random = UKismetMathLibrary::RandomIntegerInRange(0, content.Num() - 1);
		ATP_PickUp* _tempo = GetWorld()->SpawnActor<ATP_PickUp>(content[_random], FTransform((GetActorLocation())));
		if (!_tempo)return;
		_tempo->SetLifeSpan(360);
	}
}


//spawn tout le loot sur des coord au allentout proche de la box 
void ATP_LootBox::SpawnAllLoot()
{
	//UTP_TankCppTestGameInstance* _instance = Cast<UTP_TankCppTestGameInstance>(GetGameInstance());
	//if (_instance->GetDifficulty() > difficulty)return;
	for (size_t i = 0; i < content.Num(); i++)
	{
		float _Xoffset = UKismetMathLibrary::RandomFloatInRange(-100, 100);
		float _Yoffset = UKismetMathLibrary::RandomFloatInRange(-100, 100);
		ATP_PickUp* _tempo=GetWorld()->SpawnActor<ATP_PickUp>(content[i], FTransform (GetActorLocation()+FVector (_Xoffset, _Yoffset, 0)));
		if (!_tempo)return;
		_tempo->SetLifeSpan(360);
	}
}


void ATP_LootBox::DestroyLootBox()
{
	onBoxDestroyed.Broadcast();
	Destroy();
}

