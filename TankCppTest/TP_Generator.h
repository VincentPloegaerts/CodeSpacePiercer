#pragma once

/*
* Mostly Nassim and Vincent
* First type of boss encountered 
* use a protection system that makes it untouchable when there's still enemies around
* through event and subscribing to the room it gets when an actor dies and update itself
* there's niagara and effect that activate when it dies
*/

#include "CoreMinimal.h"
#include "TP_EnemyBoss.h"
#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"
#include "TP_Generator.generated.h"

class ATP_Room;
class UTP_DialogueUI;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLittleExplosion);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLastExplosion);


UCLASS()
class TANKCPPTEST_API ATP_Generator : public ATP_EnemyBoss
{
	GENERATED_BODY()

	UPROPERTY()
		UTP_DialogueUI* dialogueUI = nullptr;
	UPROPERTY(EditAnywhere, Category = "Generator | Text")
		TArray<FString> destroyText;
	UPROPERTY(EditAnywhere, Category = "Generator | Text")
		TArray<FString> spawnTankText;

protected:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnLittleExplosion onLittleExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnLastExplosion onLastExplosion;

	UPROPERTY()
		FTimerHandle spawnTankTimer;
	UPROPERTY(EditAnywhere)
		USceneComponent* positionSpawnTank = nullptr;
	UPROPERTY(EditAnywhere,Category = "Generator | Parameter")
		TSubclassOf<class ATP_TankEnemy> tankType = nullptr;
	UPROPERTY(EditAnywhere, Category = "Generator | Parameter")
		int amountSpawnEnemy = 1;
	//le temps que met le tank a respawn quand le shield se repare
	UPROPERTY(EditAnywhere, Category = "Generator | Parameter")
		float timeSpawnTank = 2;
	
	UPROPERTY(EditAnywhere, Category = "Generator | Visual | FX")
		UNiagaraSystem* explosionFX = nullptr;
	UPROPERTY(EditAnywhere, Category = "Generator | Visual | FX")
		UNiagaraSystem* lastExplosionFX = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator | Visual | FX")
		float explosionFXScale = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator | Visual | FX")
		float explosionFXDuration = 5;
	UPROPERTY(EditAnywhere, Category = "Generator | Visual | FX")
		float explosionFXRate = 0.1;
	UPROPERTY(EditAnywhere, Category = "Generator | Visual | FX")
		FVector explosionRange = FVector::OneVector * 100;
	UPROPERTY(EditAnywhere, Category = "Generator | Visual | Mesh")
		UStaticMesh* meshAfterExplosion = nullptr;
	UPROPERTY(EditAnywhere, Category = "Generator | Visual | Mesh")
		UMaterialInterface* materialAfterExplosion = nullptr;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* staticMesh = nullptr;
	UPROPERTY()
		FTimerHandle meshTimer;
	UPROPERTY()
		FTimerHandle explosionTimer;
	UPROPERTY()
		FTimerHandle explosionStopTimer;
	UPROPERTY()
		FTimerHandle lerpTimer;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Generator | Visual | Mesh")
		float timeSwitchMesh = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator | Visual | Emissive")
		float timeBeforeEmissive = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator | Visual | Emissive")
		float EmissiveLerpUpDuration = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator | Visual | Emissive")
		float EmissiveLerpDownDuration = 0.2;
	UPROPERTY()
		float EmissiveInitialValue = 0;
	UPROPERTY(EditAnywhere, Category = "Generator | Visual | Emissive")
		float EmissiveMaxValue = 20000;
	UPROPERTY()
		bool emissiveLerping = false;
	UPROPERTY()
		float emissiveLerpingTime = 0;
	UPROPERTY()
		float LerpValue = 0;
	UPROPERTY()
		UMaterialInstanceDynamic* material;
	
	

public:
	virtual void Activate() override;
	UFUNCTION() void SetEmissiveLerping(bool _status);
public:
	ATP_Generator();
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void RestoreShield() override;
	void SpawnTank();
	void Init(UWorld* _world) override;
	void ActorDeath(AActor* _deadActor) override;
	virtual void DefenderDestroy() override;
	UFUNCTION() void ChangeMesh();
	UFUNCTION()	void DestroyText();
	UFUNCTION() void SpawnTankText();
	UFUNCTION() void GenerateExplosionFX(UWorld* _world);
	UFUNCTION() void StopExplosionFX(UWorld* _world);
	void EmissiveBehaviour();
	
};