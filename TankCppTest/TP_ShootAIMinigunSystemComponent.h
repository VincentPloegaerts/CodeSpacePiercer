#pragma once

#include "CoreMinimal.h"
#include "TP_ShootMinigunSystemComponent.h"
#include "TP_ShootAIMinigunSystemComponent.generated.h"

UCLASS()
class TANKCPPTEST_API UTP_ShootAIMinigunSystemComponent : public UTP_ShootMinigunSystemComponent
{
	GENERATED_BODY()
	//SkeletalMesh Stokage
	UPROPERTY()
		USkeletalMeshComponent* skeletalMesh = nullptr;
	//amount of Bones in skeletalMesh
	UPROPERTY()
		int amountBone = 0;
	//Array of bonesName
	UPROPERTY()
		TArray<FName> boneNames;
	UPROPERTY()
		class ATP_TankPlayer* target = nullptr;
public:
	virtual void Shoot();
public:
	UTP_ShootAIMinigunSystemComponent();
	FORCEINLINE float GetShootRate() {return shootRate;}
	FORCEINLINE void SetShootRate(float _newRate) {shootRate = _newRate;}

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
