#pragma once

//code majoriterement ecrit par julie

/*Class mere des pick up life et des pick up ammo
la class est tres generique et n'as pas encore le type de pick up que ce seras*/

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_HealthComponent.h"
#include "TP_TankPlayer.h"
#include "TP_SplineActor.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "TP_PickUp.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_PickUp : public AActor
{
	GENERATED_BODY()
protected:
	//event pas utilisr dans le programme a part peut etre dans les bp des pick up
		DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroy);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnDestroy onDestroy;

#pragma region Meshes
	//deux static mesh a cause des pick up plasma 
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* staticMesh = nullptr;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* secondMesh = nullptr;
	//le niagara component trail sert lorsque les pick up bouge ca creer une ligne derriere
	UPROPERTY(VisibleAnywhere)
		UNiagaraComponent* trail = nullptr;
	//le niagara spe creer l'effet de scintillement autotu du pick up
	UPROPERTY(VisibleAnywhere)
		UNiagaraComponent* spe = nullptr;
	//cette box n'est pas du tout utiliser
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* colisionBox = nullptr;
#pragma endregion

#pragma region  Variables
	UPROPERTY(EditAnywhere, Category = "PickUp | Attract Range")
		float attractRange = 750;
	UPROPERTY(EditAnywhere, Category="PickUp | CheckTime")
		float checkTimer = 0.5;
	UPROPERTY()
		float lerpTime = 0;
	UPROPERTY()
		FRotator rotation = FRotator::ZeroRotator;
	UPROPERTY()
		FVector startLocation = FVector::ZeroVector;
#pragma endregion
protected:
	//notre pick up suit une spline donc on la stock
	UPROPERTY(EditAnywhere)
		ATP_SplineActor* _spline = nullptr;
	//nous permet de stocker l'adresse du player
	UPROPERTY()
		ATP_TankPlayer* player=nullptr;

public:
	//accesseur pour savoir si le joueur se trouve dans le range du pick up 
	FORCEINLINE bool IsPlayerInRange() { return (FVector::Dist(GetActorLocation(), player->GetActorLocation()))<attractRange; }
	//accesseur de l'event 
	FORCEINLINE FOnDestroy& OnDestroy() { return onDestroy; }
public:	
	ATP_PickUp();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
protected:
	//se diriger vers le joueur
	void GoToPlayer();
	//fonction d'affichage du debug
	void DrawPickUpRange();
	void GetPlayer();
	void TimeProgress();
	bool CheckPickUpPhysics();
	//behavior general des pick up qui va etre la meme pour la vie et les munitions
	void Behavior();

	//fonction inutiliser mais qui a etait creer pour le moment du spawn du pick up
	FRotator GetRandomRotator();

public:
	//le pick up suit une spline en 3 points et lors de son arriver sur le joueur on le detruit
	void DestroySpline();
	//on hide tout les mesh et les niagaras du pick up sauf pour le trail qui disparais apres un tres court delai
	void HidePickUp();
	bool GetLerpTime();
};
