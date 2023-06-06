#include "TP_PickUpAmmoComponent.h"
#include "TP_PickUpAmmo.h"
#include "TP_TankPlayer.h"

UTP_PickUpAmmoComponent::UTP_PickUpAmmoComponent()
{

}

//on override le bhavior du pick up component pour que selon le ammo type 
void UTP_PickUpAmmoComponent::Behavior(AActor* _targetOverlap)
{
	ATP_TankPlayer* _target = Cast<ATP_TankPlayer>(_targetOverlap);
	if (!_target)return;
	ATP_PickUpAmmo* _tempo = Cast<ATP_PickUpAmmo>(GetOwner());
	if (!_tempo)return;
	if (ammoType == PlasmaAmmo)
	{
		UTP_ShootPlasmaComponent* _shootPlasma = _target->GetShootPlasmaComponent();
		if (!_shootPlasma)return;
		if (_shootPlasma->FullPlasmaAmmo() && _tempo->GetLerpTime())return;
		_shootPlasma->AddAmmo(ammoNumber);
	}
	if (ammoType == NukeAmmo)
	{
		UTP_ShootNukeComponent* _shootNuke = _target->GetShootNukeComponent();
		if (!_shootNuke)return;
		if (_shootNuke->FullNukeAmmo() && _tempo->GetLerpTime())return;
		_shootNuke->AddAmmo(ammoNumber);
	}
	Super::Behavior(_targetOverlap);
}