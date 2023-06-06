#pragma once
/*===========================================================
* Avenant Remi
* prog 1 2020/2021
*
* Jump pad system, used to make entity jump (drone and player)
*
===========================================================*/
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TP_GameCharacter.h"
#include "NiagaraComponent.h"
#include "TP_TankPlayer.h"
#include "TP_Jump_Pad.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_Jump_Pad : public AActor
{
	GENERATED_BODY()
#pragma region Event
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventJumpadActivateForPlayer);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventJumpadActivateForPlayer onJumpadActivateForPlayer;
#pragma endregion
private:
#pragma region Component
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* padMesh = nullptr;

	UPROPERTY(EditAnywhere)
		UNiagaraComponent* padFX1 = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* padFX2 = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* padFX3 = nullptr;
	UPROPERTY(EditANywhere, Category = "Pad | Niagara")
		UNiagaraSystem* niagaraActivation = nullptr;
	UPROPERTY(EditANywhere, Category = "Pad | Niagara")
		UNiagaraSystem* niagaraDesactivation = nullptr;
	UPROPERTY(EditAnywhere, Category = "Pad | boxColision")
		UBoxComponent* boxComponent = nullptr;
#pragma endregion

	/// Propulsion power of the jumpad
	UPROPERTY(EditAnywhere, Category = "Pad | power")
		float jumpadPower = 100000;
	UPROPERTY()
		ATP_TankPlayer* player = nullptr;
public:	
	ATP_Jump_Pad();
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Pad | Activation")
		bool isActive = true;
public:
	FORCEINLINE FEventJumpadActivateForPlayer& OnJumpadActivateForPlayer() { return onJumpadActivateForPlayer; };
	FORCEINLINE bool IsActive() { return isActive; }
	/// <summary>
	/// self explenatory, activate the jump pad
	/// </summary>
	void ActivateJumpPad();
	/// <summary>
	/// self explenatory, desactivate the jump pad
	/// </summary>
	void DesactivateJumpPad();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	/// <summary>
	/// make the player or the drone jump (add velocity up)
	/// </summary>
	UFUNCTION()	void Bump(AActor* _me, AActor* _other);
};
