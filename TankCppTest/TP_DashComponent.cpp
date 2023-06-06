#include "TP_DashComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TP_TankPlayer.h"
#include "TP_Drone.h"

UTP_DashComponent::UTP_DashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UTP_DashComponent::BeginPlay()
{
	Super::BeginPlay();
	FindOwnerMovementComponent();
	onDash.AddDynamic(this, &UTP_DashComponent::timeKB);

}
void UTP_DashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TimeProgress();
	KnockBack();
}

void UTP_DashComponent::timeKB()
{
	ATP_TankPlayer* _tonk = Cast<ATP_TankPlayer>(GetOwner());
	USphereComponent* kbZone = _tonk->GetKbSphere();
	kbZone->SetWorldScale3D(FVector(sphereKBSize, sphereKBSize, sphereKBSize));

	if (vertical == 0 && horizontal == 0)
		kbZone->SetRelativeLocation(FVector(160, 0, 0));
	else if (horizontal != 0)
		kbZone->SetRelativeLocation(FVector(0, 90, 0)* horizontal);
	else if (vertical != 0)
		kbZone->SetRelativeLocation(FVector(160, 0, 0)* vertical);

	FTimerHandle _kbTimer;
	GetWorld()->GetTimerManager().SetTimer(_kbTimer, this, &UTP_DashComponent::kbZoneOff, 0.75, false);

}
void UTP_DashComponent::kbZoneOff()
{
	ATP_TankPlayer* _tonk = Cast<ATP_TankPlayer>(GetOwner());
	USphereComponent* kbZone = _tonk->GetKbSphere();
	kbZone->SetWorldScale3D(FVector(0, 0, 0));
	kbZone->SetRelativeLocation(FVector(0, 0, 0));
	dashKBOn = false;
}
void UTP_DashComponent::KnockBack()
{
	if (!dashKBOn)return;
	ATP_TankPlayer* _tonk = Cast<ATP_TankPlayer>(GetOwner());
	USphereComponent* kbZone = _tonk->GetKbSphere();
	TArray<AActor*> _toKB;
	TSubclassOf<ATP_Drone> _filter;
	kbZone->GetOverlappingActors(_toKB, _filter);
	for (size_t i = 0; i < _toKB.Num(); i++)
	{
		ATP_Drone* _drone = Cast<ATP_Drone>(_toKB[i]);
		if (!_drone)continue;
		if (_drone->GetCharacterMovement()->Velocity.Size() > maxVelocity)continue;
		_drone->wasBumped = true;
		FVector _kb = _drone->GetActorForwardVector() * -kbPower;
		_kb.Z = kbBumpUp;
		_drone->GetCharacterMovement()->AddImpulse(_kb, true);
		_drone->GetAiMovement()->Stunned(0.5);

	}
}

void UTP_DashComponent::FindOwnerMovementComponent()
{
	charaComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
}

void UTP_DashComponent::TimeProgress()
{
	if (canDash)return;
	time = time + GetWorld()->DeltaTimeSeconds;
	if (IsCoolDawnDone())
	{
		canDash = true;
		time = 0;
		return;
	}
}
void UTP_DashComponent::Dash()
{
	if (!canDash || !isActivated)return;
	
	ATP_TankPlayer* _owner = Cast<ATP_TankPlayer>(GetOwner());
	if (!_owner || !_owner->IsActive())return;
	canDash = false;
	onDash.Broadcast();
	dashKBOn = true;
	FTimerHandle _dashA;
	FTimerHandle _dashB;

	if (vertical == 0 && horizontal == 0)
		GetWorld()->GetTimerManager().SetTimer(_dashA, this, &UTP_DashComponent::BaseDash, delayedDash, false);
	if(horizontal != 0)
		GetWorld()->GetTimerManager().SetTimer(_dashA, this, &UTP_DashComponent::DashSideway, delayedDash, false);
	if (vertical!=0)
		GetWorld()->GetTimerManager().SetTimer(_dashB, this, &UTP_DashComponent::DashForward, delayedDash, false);

	if (dashEffect)
		UNiagaraFunctionLibrary::SpawnSystemAttached(dashEffect, _owner->GetMeshTankBody(), boneName,FVector() + offSet, offSetRotator,EAttachLocation::SnapToTarget,false);
}
void UTP_DashComponent::DashForward()
{
	AActor* _owner = GetOwner();
	if (!_owner || !charaComponent)return;
	currentDash = (vertical/FMath::Abs(vertical)) * _owner->GetActorForwardVector() * dashPower + FVector(0, 0, jumpPower);
	charaComponent->AddImpulse(currentDash, true);
	FTimerHandle _dashSlowTimer;
	GetWorld()->GetTimerManager().SetTimer(_dashSlowTimer, this, &UTP_DashComponent::DashSlow, delayBeforDashSlow, false);
}
void UTP_DashComponent::DashSideway()
{
	AActor* _owner = GetOwner();
	if (!charaComponent || !_owner)return;
	currentDash = (horizontal/FMath::Abs(horizontal)) * _owner->GetActorRightVector() * dashPower + FVector(0, 0, jumpPower);
	charaComponent->AddImpulse(currentDash, true);
	FTimerHandle _dashSlowTimer;
	GetWorld()->GetTimerManager().SetTimer(_dashSlowTimer, this, &UTP_DashComponent::DashSlow, delayBeforDashSlow, false);
}
void UTP_DashComponent::BaseDash() 
{ 
	AActor* _owner = GetOwner();
	if (!charaComponent || !_owner)return;
	currentDash = _owner->GetActorForwardVector() * dashPower + FVector(0, 0, jumpPower);
	charaComponent->AddImpulse(currentDash, true);
	FTimerHandle _dashSlowTimer;
	GetWorld()->GetTimerManager().SetTimer(_dashSlowTimer, this, &UTP_DashComponent::DashSlow, delayBeforDashSlow, false);
}
void UTP_DashComponent::DashSlow()
{
	if (!charaComponent)return;
	charaComponent->Velocity /= dashSpeedDivider;
}