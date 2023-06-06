#pragma once

//code majoriterement ecrit par julie

/*class mere des ammo et health component*/

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_PickUpComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKCPPTEST_API UTP_PickUpComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UTP_PickUpComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//behavior qui seras override par les components enfants
	virtual void Behavior(AActor* _targetOverlap);
private:
	
	UFUNCTION() virtual void Pick(AActor* _mine, AActor* _other);
};
