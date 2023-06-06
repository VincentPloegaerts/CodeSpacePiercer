#pragma once
/*===========================================================
* Avenant Remi
* prog 1 2020/2021
*
* Dialogue system, used to display image and text to the player
*
===========================================================*/
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "TP_StructDialogue.h"
#include "TP_DialogueUI.generated.h"

class ATP_DialogueManager;
UCLASS()
class TANKCPPTEST_API UTP_DialogueUI : public UUserWidget
{
	GENERATED_BODY()

protected:
		DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTextMove,bool,isOut);
		UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
			FOnTextMove onTextMove;

		DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginWriting);
		UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
			FOnBeginWriting onBeginWriting;

		DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishedWriting);
		UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
			FOnFinishedWriting onFinishedWriting;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* CanvasPanel_0;
	UPROPERTY(VisibleAnywhere, BlueprintREadWrite, meta = (BindWidget))
		UCanvasPanel* CanvasPanel_63;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* BackgroundImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* CharacterImage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* SpeechText;
	UPROPERTY()
		ATP_DialogueManager* dialogueManager;
	UPROPERTY()
		float timer=0;
	//time between each letter
	UPROPERTY(EditAnywhere)
		float letterRate=0.01;
	//time before the first letter (use to let time for the slide)
	UPROPERTY(EditAnywhere)
		float timeBeforeFirstLetter = 2;
	UPROPERTY(EditAnywhere)
		float timeBeforeClear = 2;
	UPROPERTY(EditAnywhere)
		float timeBeforeClearAfterSlideBackUp = 1;

	UPROPERTY()
		FString textStored;
	UPROPERTY()
		FString textDisplayed = "";
	UPROPERTY()
		int numberOfLetterDisplayed = 0;
	UPROPERTY()
		bool isTextOut =false;

	UPROPERTY()
		bool cleared = true;

	UPROPERTY()
	TArray<FDialogueStruct> dialoguesStored;

	UPROPERTY()
	FTimerHandle timerClear;
	UPROPERTY()
		bool clearImage = true;
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/// <summary>
	/// use the key to find the text and display it, will call the slide function if needed, otherwise, will store the text
	/// </summary>
	UFUNCTION() void SetSpeechText(FString _key);
	/// <summary>
	/// used to move the text box 
	/// </summary>
	void Slide(bool newStatus);
	/// <summary>
	/// clear the text instantly
	/// </summary>
	UFUNCTION() void ClearText();
	/// <summary>
	/// delay to clear the text after the box slide back up (out of the screen)
	/// </summary>
	UFUNCTION() void ClearAfterSlide();
};
