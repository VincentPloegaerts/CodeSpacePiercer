#pragma once

//code majoriterement ecrit par julie

//shield actor qui est spawn sur tous les enemies et sur le player a la position de chacun 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "TP_ShieldActor.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_ShieldActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
		UNiagaraComponent* shieldFx = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraSystem* shieldExplosionFx = nullptr;
	UPROPERTY()
		UNiagaraSystem* ShieldSphereFx = nullptr;
public:	
	ATP_ShieldActor();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	//fonction qui hide et show le shield lorsque qu'il est caché ou regen 
	UFUNCTION() void HideShield();
	UFUNCTION() void ShowShield();
public:
	//permet de mettre la scale du shield a la meme scalme que la collision sphere sur les ennemi (pas utiliser il me semble)
	void SetShieldScale(FVector _scale);
	//fonction qui permette de mettre a jour des varaibles sur le niagara du shield
	UFUNCTION() void SetShieldCrackVisibility(float _visibility,float _maxVisibility);
	void SetShieldFlash(float _flashValue);
	void ResetShieldFlash();
};
