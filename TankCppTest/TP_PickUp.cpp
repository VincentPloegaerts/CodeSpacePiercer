#include "TP_PickUp.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "TP_GameMode.h"



ATP_PickUp::ATP_PickUp()
{
	PrimaryActorTick.bCanEverTick = true;
	//RootComponent = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("RootComponent"));
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	staticMesh->SetupAttachment(RootComponent);
	secondMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondStaticMesh"));
	secondMesh->SetupAttachment(staticMesh);
	trail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraTrail"));
	trail->SetupAttachment(staticMesh);
	spe = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSpe"));
	spe->SetupAttachment(staticMesh);
}

//je commence a simulé la pysics et je set les variables qui faut pour que leur comportement soit coherent
void ATP_PickUp::BeginPlay()
{
	Super::BeginPlay();
	GetPlayer();
	staticMesh->SetSimulatePhysics(true);
	staticMesh->SetLinearDamping(10);
	staticMesh->SetMassOverrideInKg(NAME_None, 999999999999, true);
	staticMesh->SetConstraintMode(EDOFMode::SixDOF);
}

void ATP_PickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//me permet de mettre a jour la position et si il le faut la rotation , en fonction de la spline 
void ATP_PickUp::GoToPlayer()
{
	FVector _position = _spline->LocationAtTime(lerpTime);
	FRotator _rotation = FMath::Lerp(GetActorRotation(), rotation, lerpTime);
	SetActorLocation(_position);
	SetActorRotation(_rotation);
}

//fonction de debug
void ATP_PickUp::DrawPickUpRange()
{
	DrawDebugCircle(GetWorld(), GetActorLocation(), attractRange, 50, FColor::Magenta, false, -1, 0, 3, FVector(0, 1, 0), FVector(1, 0, 0));
}

//on obtiens l'adresse du player si il est instancier
void ATP_PickUp::GetPlayer()
{
	player = Cast<ATP_TankPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!player)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("Le joueur n'as pas etais detecter par les pickUp"));
}

//ca met a jour la position et le comportement de tout le pick up 
//on met aussi a jour le point final de la spline en fonction de la position du joueur
void ATP_PickUp::TimeProgress()
{
	if (IsPlayerInRange() || lerpTime>0)
	{
		lerpTime += GetWorld()->DeltaTimeSeconds;
		startLocation = GetActorLocation();
		if (_spline)return;
		_spline = GetWorld()->SpawnActor<ATP_SplineActor>(ATP_SplineActor::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);
		DisableComponentsSimulatePhysics();
		staticMesh->SetConstraintMode(EDOFMode::None);
		if (!_spline)return;
		_spline->SetupStartPoint(GetActorLocation());
		_spline->SetupMedianPoint(GetActorLocation(), player->GetActorLocation());
		_spline->SetupEndPoint(player->GetActorLocation());
		DisableComponentsSimulatePhysics();
	}
}

//renvoi si la physic et simuler sur notre pick up
bool ATP_PickUp::CheckPickUpPhysics()
{
	return staticMesh->IsSimulatingPhysics();
}

//behavior commune a tout les pick ups
void ATP_PickUp::Behavior()
{
	TimeProgress();
	if (!_spline)return;
	_spline->UpdateEndPoint(player->GetActorLocation());
	if (!IsPlayerInRange())return;
	GoToPlayer();
	if (!GetWorld()->GetAuthGameMode<ATP_GameMode>()->GetDebugDraw())return;
	DrawPickUpRange();
}


FRotator ATP_PickUp::GetRandomRotator()
{
	float _X = FMath::RandRange(0, 360);
	float _Y = FMath::RandRange(0, 360);
	float _Z = FMath::RandRange(0, 360);

	return FRotator(_X, _Y, _Z);
}

//destruction de la spline si elle existe encore
void ATP_PickUp::DestroySpline()
{
	if (!_spline)return;
	_spline->Destroy();
}

//j'hide tout les mesh du pick up sauf le trail
void ATP_PickUp::HidePickUp()
{
	staticMesh->SetHiddenInGame(true);
	secondMesh->SetHiddenInGame(true);
	spe->SetHiddenInGame(true);
	onDestroy.Broadcast();
}

// permet de savoir si le timer du lerp du pick up a deja commencer
bool ATP_PickUp::GetLerpTime()
{
	return lerpTime==0;
}


