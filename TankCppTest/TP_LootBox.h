#pragma once

//code majoriterement ecrit par julie

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_PickUp.h"
#include "TP_TankCppTestGameInstance.h"
#include "TP_LootBox.generated.h"

//enum de difficulté pour savoir quand la box seras presente
UENUM()
enum EDifficulty
{
	Easy UMETA(DisplayName = "Easy"),
	Normal UMETA(DisplayName = "Normal"),
	Hard UMETA(DisplayName = "Hard"),
};

UCLASS()
class TANKCPPTEST_API ATP_LootBox : public AActor
{
	GENERATED_BODY()
protected:
	//event appelere en bp
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoxDestroyed);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnBoxDestroyed onBoxDestroyed;
private:
#pragma region Meshes
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* baseMesh = nullptr;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* topMesh = nullptr;
#pragma endregion
	//stocke le loot qui seras spawn et qui est choisi dans le bp
	UPROPERTY(EditAnywhere, Category = "LootBox | PickUps Type")
		TArray<TSubclassOf<ATP_PickUp>> content;
	UPROPERTY(EditAnywhere, Category = "LootBox | PickUp Number In Box")
		int lootNumber = 1;
	UPROPERTY(EditAnywhere, Category = "LootBox | Is PickUp Random")
		bool isRandom = true;
	//permet en bp de choisir la difficulté dans laquelle apparaitra la loot box
	UPROPERTY(EditAnywhere, Category = "Difficulty")
		TEnumAsByte<EDifficulty> difficulty;

public:
	//accesseur de l'event
	FORCEINLINE FOnBoxDestroyed& OnBoxDestroyed() { return onBoxDestroyed;}
public:	
	ATP_LootBox();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//appeler a la destrcution et peut ete choisir en bp
	UFUNCTION() void SpawnLootRandom();
	//appeler a la destrcution et peut etre choisi en bp
	UFUNCTION() void SpawnAllLoot();
public:
	//appeler quadn on tire sur la loot box
	void DestroyLootBox();
};
