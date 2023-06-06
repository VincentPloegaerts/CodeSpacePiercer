#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TP_FakeLoading.h"
#include "TP_TutoManager.generated.h"

class ATP_TutoPlatform;
class ATP_Enemy;
class USceneComponent;
class ATP_TankPlayer;
class UTP_ShootMinigunSystemComponent;
class UTP_ShootPlasmaComponent;
class UTP_ShootNukeComponent;
class UTP_DialogueUI; 
class UBoxComponent;
class UTP_DashComponent;
class UNiagaraComponent;


/*
* Mostly Vincent 
* Used to follow a set logic through the tutorial
* rly not modulable 
* just some data like nbr of spawn and stuff like that but the order is set and done
* use bool for the first phase of movement and event after that with kills
*/

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDashPracticeFinished);
UCLASS()
class TANKCPPTEST_API ATP_TutoManager : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnDashPracticeFinished onDashPracticeFinished;
#pragma region Properties

#pragma region Bool Movement Tuto
private:
	UPROPERTY()bool isForwardPressed = false;
	UPROPERTY()bool isLeftPressed = false;
	UPROPERTY()bool isBackwardPressed = false;	
	UPROPERTY()bool isRightPressed = false;
	UPROPERTY()bool isMovementTutoFinished = false;

	UPROPERTY()bool isJumpPressed = false;
	UPROPERTY()bool isDashPressed = false;
	UPROPERTY()bool isDashTutoFinished = false;
	UPROPERTY()bool isDashPracticeFinished = false;

	UPROPERTY()bool isMinigunPressed = false;
	UPROPERTY()bool isPlasmaPressed = false;
	UPROPERTY()bool isShootTutoFinished = false;
#pragma endregion

	UPROPERTY(EditAnywhere)
		USceneComponent* positionTPTankDashPractice = nullptr;
	UPROPERTY(EditAnywhere)
		UBoxComponent* finishBoxDashPractice = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* ArrowForDash = nullptr;
#pragma region general
	UPROPERTY() 
		ATP_TankPlayer* player = nullptr;
	UPROPERTY() 
		UTP_ShootMinigunSystemComponent* playerMinigun = nullptr;
	UPROPERTY()
		UTP_ShootPlasmaComponent* playerPlasma = nullptr;
	UPROPERTY()
		UTP_ShootNukeComponent* playerNuke = nullptr;
	UPROPERTY()
		UTP_DialogueUI* dialogueUI;
	UPROPERTY()
		UTP_DashComponent* playerDash = nullptr;
	UPROPERTY()
		FTimerHandle timer;
	UPROPERTY(VisibleAnywhere, Category = "Tuto | Array")
		TArray<ATP_Enemy*> spawnedEnemies;
	UPROPERTY(EditAnywhere, Category = "Tuto | MovementTuto")
		TArray<ATP_TutoPlatform*> PlatformTutoMovement;
	UPROPERTY(EditAnywhere, Category = "Tuto | MovementTuto")
		float downOffset = 1000.f;
	UPROPERTY(EditAnywhere, Category = "Tuto | MovementTuto")
		float TPRotationYaw = 90;
#pragma endregion

#pragma region Tuto Minigun
	UPROPERTY(EditAnywhere)
		USceneComponent* positionSpawnFirstEnemy = nullptr;
	UPROPERTY(EditAnywhere, Category = "Tuto | ShootTuto | Minigun Tuto")
		TSubclassOf<ATP_Enemy> spawnEntityFirstEnemi;
	UPROPERTY(EditAnywhere, Category = "Tuto | ShootTuto | Minigun Tuto")
		float timeSpawnFirstEnemy = 3;
#pragma endregion

#pragma region Plasma Tuto
	UPROPERTY(EditAnywhere)
		USceneComponent* positionSpawnSecondEnemy = nullptr;
	UPROPERTY(EditAnywhere, Category = "Tuto | ShootTuto | Plasma Tuto")
		TSubclassOf<ATP_Enemy> spawnEntitySecondEnemi;
	UPROPERTY(EditAnywhere, Category = "Tuto | ShootTuto | Plasma Tuto")
		float timeSpawnSecondEnemy = 3;
#pragma endregion

#pragma region Nuke Tuto
	UPROPERTY(EditAnywhere)
		USceneComponent* positionSpawnThirdEnemy = nullptr;
	UPROPERTY(EditAnywhere, Category = "Tuto | ShootTuto | Nuke Tuto")
		TSubclassOf<ATP_Enemy> spawnEntityThirdEnemi;
	UPROPERTY(EditAnywhere, Category = "Tuto | ShootTuto | Nuke Tuto")
		float timeSpawnThirdEnemy = 3;
	UPROPERTY(EditAnywhere, Category = "Tuto | ShootTuto | Nuke Tuto")
		float offsetRightSpawnThirdEnemies = 500;
	UPROPERTY(EditAnywhere, Category = "Tuto | ShootTuto | Nuke Tuto")
		float offsetForwardSpawnThirdEnemies = 0;
	UPROPERTY(EditAnywhere, Category = "Tuto | ShootTuto | Nuke Tuto")
		int numberOfEnemiesThirdTuto = 5;
#pragma endregion

#pragma region NextLevel
	UPROPERTY(EditAnywhere, Category = "Tuto | NextLevel")
		FName nextLevelName = "";
	UPROPERTY(EditAnywhere, Category = "Tuto | NextLevel")
		float timeSwitchNextLevel = 3;
	UPROPERTY(EditAnywhere, Category = "Tuto | NextLevel")
		TSubclassOf<UTP_FakeLoading> widgetLoad;
#pragma endregion

#pragma endregion

#pragma region NameInput
	UPROPERTY(EditAnywhere, Category = "TankPlayer | NameInput")
		FName moveVertical = "Move_Vertical";
	UPROPERTY(EditAnywhere, Category = "TankPlayer | NameInput")
		FName moveHorizontal = "Move_Horizontale";
	UPROPERTY(EditAnywhere, Category = "TankPlayer | NameInput")
		FName moveMouseX = "Move_Mouse_X";
	UPROPERTY(EditAnywhere, Category = "TankPlayer | NameInput")
		FName moveMouseY = "Move_Mouse_Y";
	UPROPERTY(EditAnywhere, Category = "TankPlayer | NameInput")
		FName shootGatling = "Shoot_Gatling";
	UPROPERTY(EditAnywhere, Category = "TankPlayer | NameInput")
		FName shootPlasma = "Shoot_Plasma";
	UPROPERTY(EditAnywhere, Category = "TankPlayer | NameInput")
		FName shootNuke = "Shoot_Nuke";
	UPROPERTY(EditAnywhere, Category = "TankPlayer | NameInput")
		FName dash = "Dash";
	UPROPERTY(EditAnywhere, Category = "TankPlayer | NameInput")
		FName jumpName = "Jump";
#pragma endregion

#pragma region Engine Methods
public:
	ATP_TutoManager();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
#pragma endregion

private:
	UFUNCTION() void AxisForward(float _axis);
	UFUNCTION() void AxisRight(float _axis);
	UFUNCTION() void MoveTutoFinished();

	UFUNCTION() void ActionDash();
	UFUNCTION() void ActionJump();
	UFUNCTION() void DashTutoFinished();
	void MovePlatforms(TArray<ATP_TutoPlatform*>& _platformList, FVector _movement);

	UFUNCTION() void DashPractice(AActor* _me, AActor* _other);

	UFUNCTION() void ActionMinigun();
	UFUNCTION() void ActionPlasma();
	UFUNCTION() void ShootTutoFinished();

	UFUNCTION() void SpawnFirstEnemy();
	UFUNCTION() void RemoveFirstEnemy(AActor* _deadActor);

	UFUNCTION() void SpawnSecondEnemy();
	UFUNCTION() void ActivateMinigun();
	UFUNCTION() void RemoveSecondEnemy(AActor* _deadActor);

	UFUNCTION() void SpawnThirdEnemies();
	UFUNCTION() void RemoveThirdEnemy(AActor* _deadActor);

	UFUNCTION() void Changelevel();
	UFUNCTION() void TPPlayer();

	UFUNCTION() void SetDialogue();
};
