#pragma once
/*===========================================================
* Avenant Remi
* prog 1 2020/2021
*
* Super basic component, self explenatory, a box that teleport you (neet)
* used to put the player back after a fall into "the void"
*
===========================================================*/
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TP_TeleportationBox.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_TeleportationBox : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY()
		UBoxComponent* triggerBox = nullptr;
	//coord of the teleportation
	UPROPERTY(EditAnywhere, Category = "TeleportBox ")
		FVector teleportationCoord = FVector(-1276 ,2060 ,1760);
	//rotation of the player after the teleportation, used to make the player face the right direction
	UPROPERTY(EditAnywhere, Category = "TeleportBox ", meta = (ClampMin = "0", ClampMax = "360", UIMin = "0", UIMax = "360"))
		float teleportationZRotation = 0;

	//use to stop veloctiry after a tp
	UPROPERTY(EditAnywhere, Category = "TeleportBox ")
		bool cancelVelocity = true;
	UPROPERTY(EditAnywhere, Category = "TeleportBox ")
		bool setView = true;
public:	
	ATP_TeleportationBox();
	



private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	/// <summary>
	/// teleport the player that colide with the box
	/// </summary>
	UFUNCTION() void Teleport(AActor* _mine, AActor* _other);
};
