#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "TP_HealthBarBoss.generated.h"

class ATP_Enemy;
class ATP_Generator;
UCLASS()
class TANKCPPTEST_API UTP_HealthBarBoss : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATP_Enemy* owner = nullptr;

public:
	virtual void NativeConstruct() override;
	void SetGeneratorOwner(ATP_Generator* _owner);
	UFUNCTION() void SetHealth(float _currentHealth,float _maxHealth);
	void EraseTheWidget();

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Image_45 = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Image_28 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* lifeBar = nullptr;*/

};
