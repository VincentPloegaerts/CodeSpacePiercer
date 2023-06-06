#pragma once

#include "CoreMinimal.h"
#include "TP_Bullet.h"
#include "NiagaraSystem.h"
#include "TP_NukeBullet.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_NukeBullet : public ATP_Bullet
{
	GENERATED_BODY()
	UPROPERTY();
		class ATP_GameCharacter* target = nullptr;
	UPROPERTY()
		float time = 0;
	UPROPERTY(EditAnywhere, Category = "Bullet | Stat")
		float speedCourbe = 2;
	UPROPERTY(EditAnywhere, Category = "Bullet | Stat")
		UNiagaraSystem* niagaraOnImpact = nullptr;
public:
	ATP_NukeBullet();
	void SetTarget(ATP_GameCharacter* _target);
protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void BulletBehavior() override;
	virtual void DealDammage(AActor* _me, AActor* _other) override;
};
