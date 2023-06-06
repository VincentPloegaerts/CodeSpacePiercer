#include "TP_TutoPlatform.h"

ATP_TutoPlatform::ATP_TutoPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	staticMesh->SetupAttachment(RootComponent);
}
void ATP_TutoPlatform::BeginPlay()
{
	Super::BeginPlay();
	positionInitial = GetActorLocation();
}
void ATP_TutoPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!moving)return;
	time += DeltaTime;
	SetActorLocation(FMath::Lerp(positionInitial, positionFinal, time /timeLerp));
	if (time >= timeLerp)
	{
		time = 0;
		moving = false;
	}
}

void ATP_TutoPlatform::Move(FVector _positionFinal)
{
	moving = true;
	positionFinal = _positionFinal;
	positionInitial = GetActorLocation();
}

