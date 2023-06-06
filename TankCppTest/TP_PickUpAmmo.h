#pragma once

//code majoriterement ecrit par julie

/*Class qui herite de la classe mere pick up sur laquelle on rajoute un pickUpAmmoComponent
aucune fonctiion rajouter tout est heriter de la classe mere ou provient du component*/

#include "CoreMinimal.h"
#include "TP_PickUp.h"
#include "TP_PickUpAmmoComponent.h"
#include "TP_PickUpAmmo.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_PickUpAmmo : public ATP_PickUp
{
	GENERATED_BODY()

	//on rajoute le component qui permet de definir le type de pick up entre health et ammo
	UPROPERTY(EditAnywhere)
		UTP_PickUpAmmoComponent* ammoComponent = nullptr;

public:
	ATP_PickUpAmmo();
private:
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime) override;
};
