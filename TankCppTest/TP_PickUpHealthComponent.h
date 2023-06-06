#pragma once

//code majoriterement ecrit par julie

#include "CoreMinimal.h"
#include "TP_PickUpComponent.h"
#include "TP_HealthComponent.h"
#include "TP_TankPlayer.h"
#include "TP_PickUpHealthComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TANKCPPTEST_API UTP_PickUpHealthComponent : public UTP_PickUpComponent
{
	GENERATED_BODY()
		//rajout d'une struct pour qu'on puisse ajouter la vie au player
	UPROPERTY(EditAnywhere, Category = "PickUp | Stat")
		FHealthStats statPickUp;
public:
	UTP_PickUpHealthComponent();
public:
	//on override la fonction mere pour rajouter le comportement specifique
	virtual void Behavior(AActor* _targetOverlap) override;
};
