#pragma once
#include "CoreMinimal.h"
#include "TP_StructDialogue.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FDialogueStruct 
{

	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* image = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool canRepeat = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int repeatTime = -1;
};