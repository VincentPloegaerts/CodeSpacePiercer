#include "TP_PickUpAmmo.h"
#include "TP_GameMode.h"

ATP_PickUpAmmo::ATP_PickUpAmmo()
{
	PrimaryActorTick.bCanEverTick = true;
	ammoComponent = CreateDefaultSubobject<UTP_PickUpAmmoComponent>(TEXT("PickUPAmmoComponent"));
	AddOwnedComponent(ammoComponent);
}

void ATP_PickUpAmmo::BeginPlay()
{
	Super::BeginPlay();

}

//rajout dans le tick selon le type d'Ammo de check qui permette de savoir si oui ou non le player et deja full mun et apres juste appele du behavior normal 
void ATP_PickUpAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!player || !ammoComponent)return;
	if (GetLerpTime())
	{
		switch (ammoComponent->ammoType)
		{
		case 0:
			if (player->GetShootPlasmaComponent()->FullPlasmaAmmo())return;
			break;
		case 1:
			if (player->GetShootNukeComponent()->FullNukeAmmo())return;
			break;
		default:
			break;
		}
	}
	Behavior();
}
