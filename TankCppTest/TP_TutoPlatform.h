#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_TutoPlatform.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_TutoPlatform : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* staticMesh = nullptr;
	UPROPERTY(VisibleAnywhere)
		bool moving = false;
	UPROPERTY(EditAnywhere,Category = "Tuto | Plateform")
		float timeLerp = 3;
	UPROPERTY()
		float time = 0;
	UPROPERTY()
		FVector positionFinal = FVector::ZeroVector;
	UPROPERTY()
		FVector positionInitial = FVector::ZeroVector;
	
public:
	void Move(FVector _positionFinal);
public:	
	ATP_TutoPlatform();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
