#include "TP_SplineActor.h"

ATP_SplineActor::ATP_SplineActor()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	spline->SetupAttachment(RootComponent);
}

void ATP_SplineActor::BeginPlay()
{
	Super::BeginPlay();
	spline->ClearSplinePoints();
}

void ATP_SplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATP_SplineActor::SetupStartPoint(const FVector _start)
{
	spline->AddSplinePoint(_start, ESplineCoordinateSpace::World);
}

//on setup ce point avec des random values pour que les trois point forme une courbe qui ne sera jamais la meme 
void ATP_SplineActor::SetupMedianPoint(const FVector _start, const FVector _end)
{
	float _X = FMath::RandRange(-200, 200);
	float _Y = FMath::RandRange(-300, 300);
	float _Z = FMath::RandRange(100, 400);
	FVector _median = FMath::Lerp(_start, _end, 0.5f) + FVector(_X, _Y, _Z);
	spline->AddSplinePoint(_median, ESplineCoordinateSpace::World);

}

void ATP_SplineActor::SetupEndPoint(const FVector _end)
{
	spline->AddSplinePoint(_end, ESplineCoordinateSpace::World);
}

void ATP_SplineActor::UpdateEndPoint(const FVector _end)
{
	spline->SetLocationAtSplinePoint(2, _end, ESplineCoordinateSpace::World);
}

//nous renvois la location de al spline dans le word
FVector ATP_SplineActor::LocationAtTime(const float _time)
{
	return spline->GetLocationAtTime(_time, ESplineCoordinateSpace::World);
}

