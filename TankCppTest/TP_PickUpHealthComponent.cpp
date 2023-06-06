#include "TP_PickUpHealthComponent.h"
#include "TP_PickUpHealth.h"
#include "TP_PickUp.h"

UTP_PickUpHealthComponent::UTP_PickUpHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

//pn a override la fonction et on test pour savoir si le player et full et si oui ou non il faut lui ajouter le montant de vie
void UTP_PickUpHealthComponent::Behavior(AActor* _targetOverlap)
{	
	ATP_TankPlayer* _target = Cast<ATP_TankPlayer>(_targetOverlap);
	if (!_target)return;
	UTP_HealthComponent* _healthComponent = _target->GetHealthComponent();
	if (!_healthComponent) return;
	ATP_PickUpHealth* _tempo = Cast<ATP_PickUpHealth>(GetOwner());
	if (!_tempo)return;
	if(_healthComponent->IsLifeFull() && _tempo->GetLerpTime())return;
	_healthComponent->AddStat(statPickUp.health);
	Super::Behavior(_targetOverlap);
}
