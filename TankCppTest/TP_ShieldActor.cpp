#include "TP_ShieldActor.h"

ATP_ShieldActor::ATP_ShieldActor()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	shieldFx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShieldFx"));
	shieldFx->SetupAttachment(RootComponent);

}

void ATP_ShieldActor::BeginPlay()
{
	Super::BeginPlay(); 
	ShieldSphereFx = shieldFx->GetAsset();
}

void ATP_ShieldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//met a jour la visibilité du shield selon qi il est cassé ou regen 
void ATP_ShieldActor::HideShield()
{
	if (!shieldExplosionFx || !shieldFx)return;
	shieldFx->SetAsset(shieldExplosionFx);
}

void ATP_ShieldActor::ShowShield()
{
	if (!ShieldSphereFx || !shieldFx)return;
	shieldFx->SetAsset(ShieldSphereFx);
}

void ATP_ShieldActor::SetShieldScale(FVector _scale)
{
	FTransform _test = FTransform (FRotator::ZeroRotator,FVector::ZeroVector, _scale/2);
	shieldFx->SetRelativeTransform(_test);
}


//met a jour certaine variable du niagara et sont appeler dans le shield system component
void ATP_ShieldActor::SetShieldCrackVisibility(float _visibility, float _maxVisibility)
{
	shieldFx->SetNiagaraVariableFloat("CracksVisibility", _visibility/_maxVisibility);
}

void ATP_ShieldActor::SetShieldFlash(float _flashValue)
{
	shieldFx->SetNiagaraVariableFloat("FlashShield", _flashValue);
}

void ATP_ShieldActor::ResetShieldFlash()
{
	shieldFx->SetNiagaraVariableFloat("FlashShield", 1);
}
