#pragma once

//code ecrit par julie

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "TP_SplineActor.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_SplineActor : public AActor
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		USplineComponent* spline = nullptr;
	
public:	
	ATP_SplineActor();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	//setup des troi point de ma spline
	void SetupStartPoint(const FVector _start);
	void SetupMedianPoint(const FVector _start, const FVector _end);
	void SetupEndPoint(const FVector _end);
	//update qui est appeler quand le pick up va se diriger vers le player et que le player bouge 
	void UpdateEndPoint(const FVector _end);



	FVector LocationAtTime(const float _time);

};
