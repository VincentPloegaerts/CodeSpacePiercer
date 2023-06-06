
#pragma once
/*
* Mostly Vincent
* just a trigger box linked to a room that close it when u get there
*/
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_RoomFinishLine.generated.h"

class UBoxComponent;
class ATP_Room;

UCLASS()
class TANKCPPTEST_API ATP_RoomFinishLine : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
		UBoxComponent* finishLine = nullptr;
	UPROPERTY(EditAnywhere, Category = "Finish Line | Room")
		ATP_Room* room = nullptr;

public:
	ATP_RoomFinishLine();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION() void Crossedline(AActor* _me, AActor* _other);
};
