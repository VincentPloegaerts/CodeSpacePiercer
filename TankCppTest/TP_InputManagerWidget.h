#pragma once

//code ecrit par julie et remy 

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/InputKeySelector.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Framework/Commands/InputChord.h"
#include "GameFramework/InputSettings.h"
#include "TP_InputManagerWidget.generated.h"

UCLASS()
class TANKCPPTEST_API UTP_InputManagerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	//tout les truc de l'ui dont on a besoin en code 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* allSettings = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* Keyboard = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* Controller = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Controller_B = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Keyboard_B = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Image_Preset_1 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		USlider* mouseSensitivity = nullptr;

	UPROPERTY()
		TArray<FInputAxisKeyMapping> mouseX;
	UPROPERTY()
		TArray<FInputAxisKeyMapping> mouseY;

#pragma region AllInputs

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInputKeySelector* MoveForward = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintreadWrite, meta = (BindWidget))
		UInputKeySelector* MoveBackward = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInputKeySelector* MoveRight = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInputKeySelector* MoveLeft = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInputKeySelector* ShootMinigun = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInputKeySelector* ShootPlasma = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInputKeySelector* ShootNuke = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInputKeySelector* Jump = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UInputKeySelector* Dash = nullptr;
#pragma endregion

#pragma region Tableau de touche
	//tout mes tableau de touche qui servent pour chaque action est axis mapping
	UPROPERTY(VisibleAnywhere, BlueprintreadWrite)
		TArray<FInputAxisKeyMapping> moveFront;
	UPROPERTY(VisibleAnywhere, BlueprintreadWrite)
		TArray<FInputAxisKeyMapping> moveSideway;
	UPROPERTY(VisibleAnywhere, BlueprintreadWrite)
		TArray<FInputActionKeyMapping> shoot_Nuke;
	UPROPERTY(VisibleAnywhere, BlueprintreadWrite)
		TArray<FInputActionKeyMapping> shoot_Minigun;
	UPROPERTY(VisibleAnywhere, BlueprintreadWrite)
		TArray<FInputActionKeyMapping> shoot_Plasma;
	UPROPERTY(VisibleAnywhere, BlueprintreadWrite)
		TArray<FInputActionKeyMapping> jump_;
	UPROPERTY(VisibleAnywhere, BlueprintreadWrite)
		TArray<FInputActionKeyMapping> dash_;
#pragma endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UInputSettings* ourSettings = nullptr;

	UPROPERTY()
		FTimerHandle timer;
	UPROPERTY()
		//tableau de tout les touches actueleent utilisé
		TArray<FKey> allBindedKeys;
	
	UPROPERTY(VisibleAnywhere, Category = "Map")
		TMap<FName, FInputAxisKeyMapping> allKeys;

	//valeur minimum et maximum pour la sensi de la souris
	UPROPERTY(EditAnywhere, Category = "SensiMouse")
		float max = 5;
	UPROPERTY(EditAnywhere, Category = "SensiMouse")
		float min = 0.1;

private:
	virtual void NativeConstruct()override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;
	void Init();

	//permet de changer la sensi de la souris uniquement et sur les deux axes d'un coup
	UFUNCTION() void ChangeSensi();

	//permet de changer toute les touches d'action sauf celle d'acces au menu pause et celle de retour
	void ChangeKey(FInputChord _chord, FName _actionName, TArray<FInputActionKeyMapping>& _action, UInputKeySelector* _selectorName, int _index);
	UFUNCTION() void ChangeActionKeyDash(FInputChord _chord);
	UFUNCTION() void ChangeActionKeyJump(FInputChord _chord);
	UFUNCTION() void ChangeActionKeyShootM(FInputChord _chord);
	UFUNCTION() void ChangeActionKeyShootP(FInputChord _chord);
	UFUNCTION() void ChangeActionKeyShootN(FInputChord _chord);
	bool CheckKeyFree(FKey _myKey);

	//permet de changer toutes les touches de deplacemets
	UFUNCTION() void ChangeAxiskeyForward(FInputChord _chord);
	UFUNCTION() void ChangeAxiskeyBackward(FInputChord _chord);
	UFUNCTION() void ChangeAxiskeyRight(FInputChord _chord);
	UFUNCTION() void ChangeAxiskeyLeft(FInputChord _chord);

	//permet d'afficher ou de cacher dans le menu tout les boutosn necesssaires
	UFUNCTION() void SetKeyboardControlVisible();
	UFUNCTION() void SetControllerVisible();
	UFUNCTION() void SetKeyboardInvisible();
	UFUNCTION() void SetControllerInvisible();

};
