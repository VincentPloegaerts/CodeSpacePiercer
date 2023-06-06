#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TP_Bullet.generated.h"

class ATP_TankPlayer;
//struct of Damage
USTRUCT(BlueprintType)
struct FItemStats
{
	GENERATED_BODY()
#pragma region Properties
	UPROPERTY(EditAnyWhere, Category = "Stats | Dammage | Shield Dammage", meta = (ClampMin = "0", ClampMax = "999999", UIMin = "0", UIMax = "999999"))
		float shieldDammage = 0;
	UPROPERTY(EditAnyWhere, Category = "Stats | Dammage | Health Dammage", meta = (ClampMin = "0", ClampMax = "999999", UIMin = "0", UIMax = "999999"))
		float healthDammage = 0;
#pragma endregion

	FItemStats() {};

};

UCLASS()
class TANKCPPTEST_API ATP_Bullet : public AActor
{
	GENERATED_BODY()
protected:
#pragma region Component
	//
	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* projectileMovementComponent = nullptr;
	//
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* capsuleComponent = nullptr;
	//
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* staticMesh = nullptr;
	//
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* effectBullet = nullptr;
#pragma endregion
#pragma region Stat
	//Channel Collision of bullet 
	UPROPERTY(EditAnywhere, Category = "Bullet | Stat")
		TArray<TEnumAsByte<ECollisionChannel>> channelCollision;
	//speed of bullet
	UPROPERTY(EditAnywhere, Category = "Bullet | Stat")
		float moveSpeed = 1000;
	//gravity of bullet
	UPROPERTY(EditAnywhere, Category = "Bullet | Stat")
		float gravityScale = 0;
	//distance parcouru
	UPROPERTY(EditAnywhere, Category = "Bullet | Stat")
		float distanceTraveled = 5000;
	//shield damage
	UPROPERTY(EditAnyWhere, Category = "Bullet | Stat", meta = (ClampMin = "0", UIMin = "0"))
		float shieldDammage = 0;
	//health damage
	UPROPERTY(EditAnyWhere, Category = "Bullet | Stat", meta = (ClampMin = "0", UIMin = "0"))
		float healthDammage = 0;
	//One shot the shield or not
	UPROPERTY(EditAnyWhere, Category = "Bullet | Stat")
		bool breakShield = false;
	//destroy bullet on impact
	UPROPERTY(EditAnyWhere, Category = "Bullet | Stat")
		bool destroyOnImpact = true;
	//Add impulse or not
	UPROPERTY(EditAnyWhere, Category = "Bullet | Stat")
		bool addImpulse = false;
	//Power of impulse
	UPROPERTY(EditAnyWhere, Category = "Bullet | Stat")
		float impulsePower = 0.05f;
	//power Z impulse on impact
	UPROPERTY(EditAnyWhere, Category = "Bullet | Stat")
		float upImpulse = 500.f;
	//stun or not
	UPROPERTY(EditAnyWhere, Category = "Bullet | Stat")
		bool canStun = false;
	//timer stun
	UPROPERTY(EditAnyWhere, Category = "Bullet | Stat")
		float timeStun = 0.5f;
	//Damage in Area
	UPROPERTY(EditAnyWhere, Category = "Bullet | Stat")
		bool isAOE = false;
	//Area range
	UPROPERTY(EditAnyWhere, Category = "Bullet | Stat", meta = (ClampMin = "0", UIMin = "0"))
		float AOERange = 0;
#pragma endregion
public:
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComponent() { return projectileMovementComponent; };
	FORCEINLINE float GetSpeed() { return moveSpeed; }
	FORCEINLINE void SetSpeed(float _newSpeed) { moveSpeed = _newSpeed; }
#pragma region EngineMethods
public:	
	ATP_Bullet();
protected:
	virtual void BulletBehavior();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//Damage On begin overlap
	UFUNCTION() virtual void DealDammage(AActor* _me, AActor* _other);
	//Onhit Behavior
	UFUNCTION() void OnHit(AActor* SelfActor, AActor* OtherActor, FVector  NormalImpulse, const FHitResult& Hit);
private:
#pragma endregion
};
