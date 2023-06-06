#include "TP_GameCharacter.h"

ATP_GameCharacter::ATP_GameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	healthComponent = CreateDefaultSubobject<UTP_HealthComponent>("HealthComponent");
	shieldComponent = CreateDefaultSubobject<UTP_ShieldSystemComponent>("ShieldComponent");
	widgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");
	widgetComponent->SetupAttachment(RootComponent);
	AddOwnedComponent(healthComponent);
	AddOwnedComponent(shieldComponent);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void ATP_GameCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnDestroyed.AddDynamic(this, &ATP_GameCharacter::ActorDeath);
	if (healthComponent)
		healthComponent->OnLifeChange().AddDynamic(this, &ATP_GameCharacter::UpdateMaterial);
}
void ATP_GameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ATP_GameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATP_GameCharacter::Activate()
{
	canBeTarget = true;
	shieldComponent->SetDamageable(true);
	isCharacterActive = true;

}
void ATP_GameCharacter::Desactivate()
{
	canBeTarget = false;
	isCharacterActive = false;
	shieldComponent->SetDamageable(false);
}
void ATP_GameCharacter::ActorDeath(AActor* _deadActor)
{
	
}
void ATP_GameCharacter::UpdateMaterial(float _health, float _maxHealth)
{
	
}