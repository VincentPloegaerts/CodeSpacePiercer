#pragma once

//code majoriterement ecrit par julie

/*class qui herite de la class mere PickUpComponent*/

#include "CoreMinimal.h"
#include "TP_PickUpComponent.h"
#include "TP_ShootSystemComponent.h"
#include "TP_PickUpAmmoComponent.generated.h"

//rajout d'un enum pour savoir si c'est des mun de plasma ou de nuke
UENUM()
enum EChoiceAmmo
{
	PlasmaAmmo UMETA(DisplayName="PlasmaAmmo"),
	NukeAmmo UMETA(DisplayName="NukeAmmo"),
};


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TANKCPPTEST_API UTP_PickUpAmmoComponent : public UTP_PickUpComponent
{
	GENERATED_BODY()
	//on a une struct qui permet de savoir pour le player son nombre de mun
	UPROPERTY(VisibleAnywhere, Category = "PickUpAmmo")
		FAmmoData ammoData;
	//nombre de mun qu'on va rajouter au player
	UPROPERTY(EditAnywhere, Category = "PickUpAmmo | AmmoNumber")
		int ammoNumber = 5;
public:
	//on recupere du bp des pick up de mun quel type de munition c'est
	UPROPERTY(EditAnywhere, Category = "PickUpAmmo | Ammo Type")
		TEnumAsByte<EChoiceAmmo> ammoType;
public:
	UTP_PickUpAmmoComponent();
protected:
	//behavior herité de la classe mere qu'on override
	void Behavior(AActor* _targetOverlap) override;
};
