#pragma once

//code majoriterement ecrit par julie

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TP_PickUp.h"
#include "TP_LootPince.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_LootPince : public AActor
{
	GENERATED_BODY()
protected:
	//tout les event creer qui peuvent etre appeler en bp
		DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTankNear);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnTankNear onTankNear;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClawHit);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnClawHit onClawHit;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoxHitGround);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnBoxHitGround onBoxHitGround;
private:

	//les deux trigger box qui permettent de lancer le mouvement de la pince et de dire quand la box qui tombe va se detruire
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* playerTriggerBox = nullptr;
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* groundTriggerBox = nullptr;


	
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* startPoint = nullptr;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* endPoint = nullptr;
	UPROPERTY(VisibleAnywhere);
		UStaticMeshComponent* pinceArm = nullptr;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* pinceBox = nullptr;



	UPROPERTY(EditAnywhere, Category = "Claw | HeightOffset")
		float heightOffset = 0;
	UPROPERTY(EditAnywhere, Category= "Claw | ClawSpeed", meta=(ClampMin="0.0001",ClampMax="1"))
		float clawSpeed = 0.5;
	UPROPERTY(EditAnywhere, Category = "Claw | ClawContent")
		TArray<TSubclassOf<ATP_PickUp>> content;
	UPROPERTY()
		bool playerInRange = false;
	UPROPERTY()
		float time = 0;
	UPROPERTY(EditAnywhere, Category = "Claw | ClawLoop")
		bool canLoop = false;
	UPROPERTY()
		FVector startLocation = FVector::ZeroVector;
	UPROPERTY()
		FVector endLocation = FVector::ZeroVector;
	
	
public:
	//les accesseurs des events
	FORCEINLINE FOnTankNear& OnTankNear() { return onTankNear; }
	FORCEINLINE FOnClawHit& OnClawHit() { return onClawHit; }
	FORCEINLINE FOnBoxHitGround& OnBoxHitGround() { return onBoxHitGround;}
	//FORCEINLINE FVector GetStartLocation() { return startPoint->GetComponentLocation()+FVector (0,0,heightOffset);}
	//FORCEINLINE FVector GetEndLocation() { return endPoint->GetComponentLocation() + FVector (0,0, heightOffset);}
public:	
	ATP_LootPince();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	//la translation de la pince
	void PinceMovement();
	//appeler quand le player passe la trigger box
	UFUNCTION() void StartingPinceMovement(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void TimeProgress();
	//appeler quand la box et detrute
	UFUNCTION() void LootContent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);
	//changement de sens de la translation de la pince
	void ChangePinceTranslation();
public:
	//appeler par l'event quand leplayer tire sur la pince
	UFUNCTION() void DropLootBox();
};
