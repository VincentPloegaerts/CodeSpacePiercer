#pragma once

#include "CoreMinimal.h"
#include "TP_GameCharacter.h"
#include "TP_ShootMinigunSystemComponent.h"
#include "TP_ShootPlasmaComponent.h"
#include "TP_ShootNukeComponent.h"
#include "TP_DashComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TP_PauseWidget.h"
#include "TP_TankPlayer.generated.h"

class UTP_DialogueUI;
UCLASS()
class TANKCPPTEST_API ATP_TankPlayer : public ATP_GameCharacter
{
	GENERATED_BODY()
	UPROPERTY()
		TArray<AActor*> toIgnore;
#pragma region Material
	//Material Instance Data
	UPROPERTY()
		UMaterialInstanceDynamic* materialMeshTankBody = nullptr;
	UPROPERTY()
		UMaterialInstanceDynamic* materialMeshTurret = nullptr;
	UPROPERTY()
		UMaterialInstanceDynamic* materialMeshBaseTurret = nullptr;
	//End Material Instance Data
#pragma endregion
#pragma region Event
protected:
	//Call when Player is not on the ground
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventIsGrounded);
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite)
		FEventIsGrounded onPlayerFaling;
	//Event de merde
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventReturn);
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite)
		FEventReturn onPlayerReturnMenu;
	//Call onNavHit
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventPosOnNavmesh , FVector , impactPos);
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite)
		FEventPosOnNavmesh onNavMeshHit;
	//Call on move boutton pressed
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMove);
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite)
		FOnMove onMove;
#pragma endregion
private:
#pragma region Component
	UPROPERTY(EditAnywhere)
		UCameraComponent* camera = nullptr;
	//Sphere Collision calcule le du dash KB
	UPROPERTY(EditAnywhere, Category = "TankPlayer | Kbsphere")
		USphereComponent* sphereComponent = nullptr;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		USpringArmComponent* springArm = nullptr;
private:
	UPROPERTY()
		UTP_DialogueUI* dialogueUI = nullptr;
	//Text shield break ,damage,etc
	UPROPERTY(EditAnywhere, Category = "TankPlayer | Text")
		TArray<FString> shieldText;
	//Text health break ,damage,etc
	UPROPERTY(EditAnywhere, Category = "TankPlayer | Text")
		TArray<FString> healthText;
	UPROPERTY(EditAnywhere)
		UTP_DashComponent* dashComponent = nullptr;
#pragma region SkeletalMeshRegion
	//All Skeletal
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* meshTankBody = nullptr;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* meshBaseTurret = nullptr;
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* meshTurret = nullptr;
#pragma endregion
#pragma region ShootSystem
	UPROPERTY(EditAnywhere)
		UTP_ShootMinigunSystemComponent* shootMinigunSystem = nullptr;
	UPROPERTY(EditAnywhere)
		UTP_ShootPlasmaComponent* shootPlasmaSystem = nullptr;
	UPROPERTY(EditAnywhere)
		UTP_ShootNukeComponent* shootNukeSystem = nullptr;
#pragma endregion

#pragma region Niagara
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* shootFXRight = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* shootFXRight1 = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* shootFXLeft = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* shootFXLeft1 = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* douilleEjectRight = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* douilleEjectLeft = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* trusterFront = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* trusterFront1 = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* trusterBack = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* trusterBack1 = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* trusterPulse = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* trusterPulse1 = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* trusterPulse2 = nullptr;
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* trusterPulse3 = nullptr;
#pragma endregion 
#pragma endregion
#pragma region Stat

	UPROPERTY(EditAnywhere, Category = "TankPlayer | Stat")
		float amplitudeRotationBase = 2;
	UPROPERTY(EditAnywhere, Category = "TankPlayer | Stat")
		float lerpStrenght = 0.03;
	UPROPERTY(EditAnywhere, Category = "TankPlayer | Stat")
		float moveSpeedForward = 250;
	UPROPERTY(EditAnywhere, Category = "TankPlayer | Stat")
		float moveSpeedSide = 250;

	UPROPERTY(EditAnywhere, Category = "TankPlayer | Stat")
		float rotationSpeedUp = 150;
	//Rotate Turret
	UPROPERTY(EditAnywhere, Category = "TankPlayer | Stat")
		float maxRotateTurret = 35;
	//Rotate Turret
	UPROPERTY(EditAnywhere, Category = "TankPlayer | Stat")
		float minRotateTurret = -10;
	UPROPERTY(EditAnywhere, Category = "TankPlayer | Stat")
		float maxRotateCamera = 45;
	UPROPERTY(EditAnywhere, Category = "TankPlayer | Stat")
		float minRotateCamera = -45;
	UPROPERTY(EditAnywhere, Category = "TankPlayer | Camera Effect")
		float translucentMaxCap = 0.25;
	UPROPERTY(EditAnywhere, Category = "TankPlayer | Camera Effect")
		float translucentMaxSet = 0.2;

	UPROPERTY(EditAnywhere, Category = "TankPlayer | Camera Effect")
		float translucentMinCap = 0.9;
	UPROPERTY(EditAnywhere, Category = "TankPlayer | Camera Effect")
		float translucentMinSet = 1;
protected:
	//speed of mouse rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TankPlayer | Stat")
		float rotationSpeedSide = 300;
private:
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
	UPROPERTY(EditAnywhere, Category = "TankPlayer | NameInput")
		FName pause = "Pause";
#pragma endregion
#pragma region RayCastParam
	UPROPERTY(EditAnywhere, Category = "TankPlayer | MovementOnMesh")
		float distanceToGround = 400;
	UPROPERTY()
		float backupDistanceToGround = distanceToGround;
#pragma endregion

#pragma region AxisValues
	UPROPERTY()
		float vertical = 0;
	UPROPERTY()
		float horizontal = 0;
#pragma endregion
	//widget pop on death player
	UPROPERTY(EditAnywhere, Category ="TankPlayer | WidgetOnDeath")
		TSubclassOf<UUserWidget> widgetOnDeath = nullptr;
	//widget pop on pause in game
	UPROPERTY(EditAnywhere, Category = "TankPlayer | WidgetOnPause")
		TSubclassOf<UTP_PauseWidget> widgetOnPause = nullptr;
	//Active Desactive Player
	UPROPERTY()
		bool isActive = true;
	UPROPERTY()
		bool isJumpActivated = true;
	UPROPERTY()
		bool isOpacityChanged = false;

	UPROPERTY()
		UUserWidget* pauseWidget = nullptr;
	UPROPERTY()
		UUserWidget* optionWidget = nullptr;
	UPROPERTY()
		UUserWidget* controlWidget = nullptr;
public:
	FORCEINLINE float GetVerticalAxisValue() { return vertical; }
	FORCEINLINE float GetHorizontalAxisValue() { return horizontal; }
	FORCEINLINE UCameraComponent* GetCameraComponent() { return camera; };
	FORCEINLINE UTP_ShootMinigunSystemComponent* GetShootMinigunComponent() { return shootMinigunSystem; };
	FORCEINLINE UTP_ShootPlasmaComponent* GetShootPlasmaComponent() { return shootPlasmaSystem; };
	FORCEINLINE UTP_DashComponent* GetDashComponent() { return dashComponent; };
	FORCEINLINE UTP_ShootNukeComponent* GetShootNukeComponent() { return shootNukeSystem; };
	FORCEINLINE FEventIsGrounded& OnPlayerFaling() { return onPlayerFaling; };
	FORCEINLINE FEventPosOnNavmesh& OnNavMeshHit() { return onNavMeshHit; };
	FORCEINLINE virtual USkeletalMeshComponent* GetSkeletalMesh() { return meshTurret; };
	FORCEINLINE USkeletalMeshComponent* GetMeshTankBody() { return meshTankBody; };
	FORCEINLINE FOnMove& OnMove() { return onMove;}
	FORCEINLINE bool IsActive() { return isActive; }
	FORCEINLINE USphereComponent* GetKbSphere() { return sphereComponent; }
	FORCEINLINE bool IsJumpActivated() { return isJumpActivated; }
	FORCEINLINE void SetIsJumpActivated(bool _status) { isJumpActivated = _status; }

	FORCEINLINE void SetOptionWidget(UUserWidget* _optionWidget) {optionWidget = _optionWidget;}
	FORCEINLINE void SetControlWidget(UUserWidget* _controlWidget) { controlWidget = _controlWidget;}
	FORCEINLINE void SetPauseWidget(UUserWidget* _pauseWidget) { pauseWidget = _pauseWidget;}
	
	UFUNCTION() void DesactivatePlayer();
	UFUNCTION() void ActivatePlayer();
public:
	ATP_TankPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//UpdateMaterialShield on damage and regen
	UFUNCTION() void UpdateMaterialShield(float _currentShield, float _maxShield);
	//Material Set Shield On Destroy
	UFUNCTION() void UpdateMaterialShieldOnDestroy(float _currentShield);
	//FxShoot visible
	UFUNCTION() void FxVisible();
	//FxShoot visible
	UFUNCTION() void FxVisibleWithoutReset();
	//FxShoot invisible
	UFUNCTION() void FxInvisible();
	//move 'Z','S'
	UFUNCTION() void MoveVertical(float _axis);
	//move 'Q','D'
	UFUNCTION() void MoveHorizontal(float _axis);
	//move Mouse X
	UFUNCTION() void MoveMouseX(float _axis);
	//move Mouse Y
	UFUNCTION() void MoveMouseY(float _axis);
	//text Shield Behavior
	UFUNCTION() void TextShield();
	//text Health Behavior
	UFUNCTION() void TextHealth(bool nop);
	//Pause Widget Behavior
	UFUNCTION() void PauseWidget();
	void RayCastToNavMesh();
	//Opacity Behavior on Wall or Actor between player and camera
	void OpacityChange();
public:
	UFUNCTION() void ChangeRaycastRange();
	virtual void ActorDeath(AActor* _deadActor) override;
	virtual void Jump()override;
};
