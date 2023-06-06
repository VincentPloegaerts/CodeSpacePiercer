#include "TP_PickUpHealth.h"
#include "TP_GameMode.h"

ATP_PickUpHealth::ATP_PickUpHealth()
{
	PrimaryActorTick.bCanEverTick = true;
	healthComponent = CreateDefaultSubobject<UTP_PickUpHealthComponent>(TEXT("PickUPHealthComponent"));
	AddOwnedComponent(healthComponent);
}

void ATP_PickUpHealth::BeginPlay()
{
	Super::BeginPlay();
}


//on regarde si la vie du player est full et si c'est le cas alors le pick up ne feras rien 
void ATP_PickUpHealth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLerpTime())
	{
		if (!player || !player->GetHealthComponent())return;
		if (player->GetHealthComponent()->IsLifeFull())return;
	}
	Behavior();
}
