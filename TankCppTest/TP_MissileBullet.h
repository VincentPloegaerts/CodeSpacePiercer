#pragma once
/*===========================================================
* Avenant Remi
* prog 1 2020/2021
*
* Missile bullet used in missile tower and boss lvl 3 
* contain all the math for decal, trajectory, etc
*
===========================================================*/
#include "CoreMinimal.h"
#include "TP_Bullet.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"
#include "TP_MissileBullet.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_MissileBullet : public ATP_Bullet
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* explosionFX = nullptr;
	UPROPERTY();
		FVector target = FVector::ZeroVector;
	UPROPERTY();
		FVector targetGND = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "DecalParam")
		float impactSizeDivider = 100;
	UPROPERTY(EditAnywhere, Category = "DecalParam", meta = (ClampMin = "1", ClampMax = "10", UIMin = "1", UIMax = "10"))
		float growSpeed = 2;
	UPROPERTY(EditAnywhere, Category = "DecalParam", meta = (ClampMin = "10", ClampMax = "1000", UIMin = "10", UIMax = "1000"))
		float decalComponentHightHit = 2;

	UPROPERTY(EditAnywhere)
		FName colorParamName = "BaseColor";
	UPROPERTY(EditAnywhere)
		UMaterialInstanceDynamic* materialInstance;
	//not used
	UPROPERTY(EditAnywhere, Category = "DecalParam")
		FLinearColor BaseColorExternal;
	UPROPERTY(EditAnywhere, Category = "DecalParam")
		FLinearColor BaseColorGrowing;
	//---
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> collisionSet;
	//take a point around the player in this range
	UPROPERTY(EditAnywhere)
		float randomAroundPlayer = 500;

	UPROPERTY()
		float time = 0;
	UPROPERTY(EditAnywhere, Category = "Bullet | Stat")
		float speedCourbe = 2;
	UPROPERTY(EditAnywhere, Category = "Bullet | Stat")
		float stunedTimerDrone = 2;
	UPROPERTY()
		float dist = 0;
	UPROPERTY()
		FVector size = FVector::ZeroVector;
	UPROPERTY()
		bool asHit = false;

	UPROPERTY(EditAnywhere)
		bool goThroughWall = false;

	UPROPERTY(EditAnywhere)
		FVector posToHit = FVector::ZeroVector;
	//security timer if the bullet is stuck and did not explode
	UPROPERTY(EditAnywhere)
		float dispawnTimer = 2;

	UPROPERTY()
		FHitResult targetImpactResult;
protected:
	UPROPERTY(EditAnywhere);
	UDecalComponent* targetedZone = nullptr;
	UPROPERTY(EditAnywhere);
	UDecalComponent* targetedMaxSizeZone = nullptr;
public:
	ATP_MissileBullet();
	/// <summary>
	/// set the target to the bullet (around the player pos at the moment of the shoot) and """"""anticipate"""""" the trajectory of the missile, kinda weird, not realy balistics
	/// </summary>
	void SetTarget();

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	/// <summary>
	/// do it's bullety behavior, will go up for a bit then go rushing for the targeted position
	/// </summary>
	void BulletBehavior() override;
	/// <summary>
	/// Deal damage to the 'other' tager while ignioring the launcher , deal AOE damage with sphere trace
	/// </summary>
	virtual void DealDammage(AActor* _me, AActor* _other) override;
private:
	/// <summary>
	/// used to make the rotation for the missile impact zone (only visual)
	/// </summary>
	void CalculateDecalRot();

};
