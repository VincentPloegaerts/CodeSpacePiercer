#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TP_HealthComponent.h"
#include "TP_ShieldSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TP_GameCharacter.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_GameCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
		UTP_HealthComponent* healthComponent = nullptr;
	UPROPERTY(EditAnywhere)
		UTP_ShieldSystemComponent* shieldComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "Widget")
		UWidgetComponent* widgetComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "Stat")
		bool canBeTarget = true;
	UPROPERTY()
		bool isCharacterActive = false;
public:
	FORCEINLINE UTP_HealthComponent* GetHealthComponent() { return healthComponent; }
	FORCEINLINE UTP_ShieldSystemComponent* GetShieldComponent() { return shieldComponent; }
	FORCEINLINE UWidgetComponent* GetWidgetComponent() { return widgetComponent; }
	FORCEINLINE virtual USkeletalMeshComponent* GetSkeletalMesh() { return GetMesh(); };
	FORCEINLINE virtual bool IsTargetable() { return canBeTarget; }
	FORCEINLINE bool IsCharacterActive() { return isCharacterActive; }
	FORCEINLINE void SetIsCharacterActive(bool _status) { isCharacterActive = _status; }
public:
	ATP_GameCharacter();
public:
	UFUNCTION() virtual void ActorDeath(AActor* _deadActor);
	virtual void Activate();
	virtual void Desactivate();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION() virtual void UpdateMaterial(float _health, float _maxHealth);
};
