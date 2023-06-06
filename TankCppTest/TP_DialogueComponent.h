// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_DialogueComponent.generated.h"


class UImage;
class UTP_DialogueUI;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKCPPTEST_API UTP_DialogueComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Dialogue | Text")
		FString DialogueText;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Dialogue | Image")
		UImage* DialogueImage;
	UPROPERTY()
		UTP_DialogueUI* dialogueUi;
	//pointer to ui to init in beginplay
	//maybe even a pointer to dialogue box
	UPROPERTY(EditAnywhere, Category = "Dialogue | Text", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10"))
		float timeBetweenLetters = 0.2;
	

public:	
	UTP_DialogueComponent();
public:

	//function to use to bring hud on screen
	UFUNCTION() void ShowDialogueHUD();
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
