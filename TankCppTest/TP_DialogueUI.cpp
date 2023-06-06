#include "TP_DialogueUI.h"
#include "TP_GameMode.h"
#include "TP_DialogueManager.h"

void UTP_DialogueUI::NativeConstruct()
{
	Super::NativeConstruct();
	ATP_GameMode* _tempoGamemode = Cast<ATP_GameMode>(GetWorld()->GetAuthGameMode());
	if (!_tempoGamemode)return;
	TSoftObjectPtr<ATP_GameLogic> tempoLogic = _tempoGamemode->GetGameLogic();
	if (!tempoLogic)return;
	dialogueManager = tempoLogic->GetDialogueManager();

	//SetSpeechText("test");
}

void UTP_DialogueUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (cleared == false)return;
	if (dialoguesStored.Num() > 0)
	{
		timer += GetWorld()->DeltaTimeSeconds;
		if ((timer >= letterRate && numberOfLetterDisplayed > 0) || (timer >= timeBeforeFirstLetter && numberOfLetterDisplayed == 0))
		{
			if (numberOfLetterDisplayed == 0)
				onBeginWriting.Broadcast();
			timer = 0;
			if (numberOfLetterDisplayed < textStored.Len())
			{
				textDisplayed += textStored[numberOfLetterDisplayed];
				numberOfLetterDisplayed++;
				SpeechText->SetText(FText::FromString(textDisplayed));
				if (!CharacterImage->Brush.GetResourceObject())
				{
					CharacterImage->SetBrushFromTexture(dialoguesStored[0].image);
				}
			}
			else
			{
				onFinishedWriting.Broadcast();

				textDisplayed = "";

				if (dialoguesStored.Num() != 0)
					dialoguesStored.RemoveAt(0);
				clearImage = true;
				cleared = false;

				FTimerHandle _timer;
				GetWorld()->GetTimerManager().SetTimer(_timer, this, &UTP_DialogueUI::ClearText, timeBeforeClear, false);

				return;
			}

		}
	}
}


void UTP_DialogueUI::SetSpeechText(FString _key)
{
	if (!dialogueManager)return;

	if(!dialogueManager->GetTextMap().Contains(_key))return;
	dialoguesStored.Add(dialogueManager->GetTextMap()[_key]);


	if (dialoguesStored.Num() <= 1)
	{
		clearImage = false;
		FDialogueStruct _tempo = dialoguesStored[0];
		textStored = _tempo.text;
		CharacterImage->SetBrushFromTexture(_tempo.image);
		Slide(true);
	}


}

void UTP_DialogueUI::Slide(bool newStatus)
{
	if (newStatus != isTextOut)
	{
		onTextMove.Broadcast(newStatus);
		isTextOut = newStatus;
	}

}

void UTP_DialogueUI::ClearText()
{
	cleared = true;
	if (dialoguesStored.Num() == 0)
	{
		Slide(false);

		textStored = "";
		numberOfLetterDisplayed = 0;
		GetWorld()->GetTimerManager().SetTimer(timerClear, this, &UTP_DialogueUI::ClearAfterSlide, timeBeforeClearAfterSlideBackUp, false);
	}
	else
	{
		FDialogueStruct _tempo = dialoguesStored[0];
		textStored = _tempo.text;
		numberOfLetterDisplayed = 0;
		if (!CharacterImage) 
		{ 
			return; 
		}
		CharacterImage->SetBrushFromTexture(_tempo.image);

	}

}
void UTP_DialogueUI::ClearAfterSlide()
{
	SpeechText->SetText(FText::FromString(""));
	if(clearImage)
		CharacterImage->SetBrushFromTexture(nullptr);
	clearImage = true;

}