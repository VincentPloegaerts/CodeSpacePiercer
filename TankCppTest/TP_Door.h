// Fill out your copyright notice in the Description page of Project Settings.
/*
* Mostly Vincent
* 
* Actor used to block the path in the room 
* collision activate from room when entering and
* desactivate on end
* there's a niagara system to show a lock when closing the distance if the doors is closed
*/
#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "TP_Door.generated.h"


class UBoxComponent;

UCLASS()
class TANKCPPTEST_API ATP_Door : public AActor
{
	GENERATED_BODY()
#pragma region Event
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorExplode);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnDoorExplode onDoorExplode;

public:
	FORCEINLINE FOnDoorExplode& OnDoorExplode() { return onDoorExplode; }
#pragma endregion

#pragma region Properties
private:
#pragma region Components
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* NiagaraSide1 = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* NiagaraSide2 = nullptr;
	UPROPERTY(EditAnywhere, Category = "Door | Visuals")
		UNiagaraSystem* NiagaraExplosion = nullptr;
	UPROPERTY(EditAnywhere)
		UBoxComponent* triggerBox = nullptr;
	UPROPERTY(EditAnywhere)
		UBoxComponent* collisionBox = nullptr;
#pragma endregion

#pragma region Lock Properties
	UPROPERTY(EditAnywhere, Category = "Door | Visuals")
		float lockLerpTimeMax = 3;
	UPROPERTY()
		float lockLerpTime = 0;
	UPROPERTY()
		float lockLerpValue = 0;
	UPROPERTY()
		FString lockOpacityName = "LockOpacity";
#pragma endregion

#pragma region bool
	UPROPERTY()
		bool isInside = false;
	UPROPERTY()
		bool isLerping = false;
	UPROPERTY(EditAnywhere, Category = "Door | Opening")
		bool isOpen = true;

#pragma endregion

#pragma endregion

#pragma region Engine Methods
public:
	ATP_Door();
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
#pragma endregion

#pragma region collision Box Methods
public:
	UFUNCTION() void OpenDoor();
	UFUNCTION() void CloseDoor();
	UFUNCTION() void ExplodeDoor();
#pragma endregion

#pragma region Debug
private:
	void DrawDebugDoor(UWorld* _world);
#pragma endregion

#pragma region Lock Methods
private:
	UFUNCTION() void LeaveBox(AActor* _box, AActor* _other);
	UFUNCTION() void EnterBox(AActor* _box, AActor* _other);
	void UpdateLerpValue(float DeltaTime);
	void SetLockNiagaras (float const _newValue);
#pragma endregion

};
