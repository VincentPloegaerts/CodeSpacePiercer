#pragma once

//code majoriterement ecrit par julie

/*class enfant qui herite de la class mere pickUp a laquelle on a rajouter le component qui defini le type de pick up*/

#include "CoreMinimal.h"
#include "TP_PickUp.h"
#include "TP_PickUpHealthComponent.h"

#include "TP_PickUpHealth.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_PickUpHealth : public ATP_PickUp
{
	GENERATED_BODY()

	//rajout du component qui defini le type du pick up 
	UPROPERTY(VisibleAnywhere)
		UTP_PickUpHealthComponent* healthComponent = nullptr;

public:
	ATP_PickUpHealth();
private:
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime)override;
};
