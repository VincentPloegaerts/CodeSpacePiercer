#pragma once
/*===========================================================
* Avenant Remi
* prog 1 2020/2021
*
* Dialogue storage mostly
*
===========================================================*/
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_HUD.h"
#include "Engine/Texture.h"
#include "TP_StructDialogue.h"
#include "TP_DialogueManager.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_DialogueManager : public AActor
{
	GENERATED_BODY()
protected:
	/// <summary>
	/// store every dialogues from the level
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue | struct")
		TMap<FString, FDialogueStruct> dialogues;
public:
	ATP_DialogueManager();
	FORCEINLINE TMap<FString, FDialogueStruct> GetTextMap() { return dialogues; }
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
