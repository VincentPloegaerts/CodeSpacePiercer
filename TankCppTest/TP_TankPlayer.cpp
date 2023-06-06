#include "TP_TankPlayer.h"
#include "TP_GameMode.h"
#include "TP_DialogueManager.h"
#include "TP_DialogueUI.h"
#include "TP_HUD.h"
#include "TP_OptionWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

ATP_TankPlayer::ATP_TankPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	shootMinigunSystem = CreateDefaultSubobject<UTP_ShootMinigunSystemComponent>("ShootMinigun");
	shootPlasmaSystem = CreateDefaultSubobject<UTP_ShootPlasmaComponent>("ShootPlasma");
	shootNukeSystem = CreateDefaultSubobject<UTP_ShootNukeComponent>("ShootNuke");
	dashComponent = CreateDefaultSubobject<UTP_DashComponent>("DashComponent");
	springArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	meshBaseTurret = CreateDefaultSubobject<UStaticMeshComponent>("MeshBaseTurret");
	meshTurret = CreateDefaultSubobject<USkeletalMeshComponent>("MeshTurret");
	shootFXRight = CreateDefaultSubobject<UNiagaraComponent>("shootFxRight");
	shootFXRight1 = CreateDefaultSubobject<UNiagaraComponent>("shootFxRight1");
	shootFXLeft = CreateDefaultSubobject<UNiagaraComponent>("shootFxLeft");
	shootFXLeft1 = CreateDefaultSubobject<UNiagaraComponent>("shootFxLeft1");
	douilleEjectRight = CreateDefaultSubobject<UNiagaraComponent>("douilleFxRight");
	douilleEjectLeft = CreateDefaultSubobject<UNiagaraComponent>("douilleFxLeft");
	trusterFront = CreateDefaultSubobject<UNiagaraComponent>("trusterFront");
	trusterFront1 = CreateDefaultSubobject<UNiagaraComponent>("trusterFront1");
	trusterBack = CreateDefaultSubobject<UNiagaraComponent>("trusterBack");
	trusterBack1 = CreateDefaultSubobject<UNiagaraComponent>("trusterBack1");
	trusterPulse = CreateDefaultSubobject<UNiagaraComponent>("trusterPulse");
	trusterPulse1 = CreateDefaultSubobject<UNiagaraComponent>("trusterPulse1");
	trusterPulse2 = CreateDefaultSubobject<UNiagaraComponent>("trusterPulse2");
	trusterPulse3 = CreateDefaultSubobject<UNiagaraComponent>("trusterPulse3");
	sphereComponent = CreateDefaultSubobject<USphereComponent>("KBSphere");
	sphereComponent->SetupAttachment(RootComponent);
	meshBaseTurret->SetupAttachment(GetMesh());
	meshTurret->SetupAttachment(meshBaseTurret);
	springArm->SetupAttachment(RootComponent);
	shootFXRight->SetupAttachment(meshTurret);
	shootFXRight1->SetupAttachment(meshTurret);
	shootFXLeft->SetupAttachment(meshTurret);
	shootFXLeft1->SetupAttachment(meshTurret);
	douilleEjectRight->SetupAttachment(shootFXRight);
	douilleEjectLeft->SetupAttachment(shootFXLeft);
	trusterFront->SetupAttachment(GetMesh());
	trusterFront1->SetupAttachment(GetMesh());
	trusterBack->SetupAttachment(GetMesh());
	trusterBack1->SetupAttachment(GetMesh());
	trusterPulse->SetupAttachment(GetMesh());
	trusterPulse1->SetupAttachment(GetMesh());
	trusterPulse2->SetupAttachment(GetMesh());
	trusterPulse3->SetupAttachment(GetMesh());
	camera->SetupAttachment(springArm);
	springArm->bUsePawnControlRotation = true;
}
void ATP_TankPlayer::BeginPlay()
{
	Super::BeginPlay();
	OnDestroyed.RemoveDynamic(this, &ATP_TankPlayer::ActorDeath);
	meshTankBody = GetMesh();
	shootFXRight->Deactivate();
	shootFXRight1->Deactivate();
	shootFXLeft->Deactivate();
	shootFXLeft1->Deactivate();
	douilleEjectLeft->Deactivate();
	douilleEjectRight->Deactivate();
	materialMeshTankBody = meshTankBody->CreateDynamicMaterialInstance(0);
	materialMeshTurret = meshTurret->CreateDynamicMaterialInstance(0);
	materialMeshBaseTurret = meshBaseTurret->CreateDynamicMaterialInstance(0);
	if (!shootMinigunSystem)return;
	shootMinigunSystem->OnStartShootEvent().AddDynamic(this, &ATP_TankPlayer::FxVisible);
	shootMinigunSystem->OnShootEvent().AddDynamic(this, &ATP_TankPlayer::FxVisibleWithoutReset);
	shootMinigunSystem->OnStopShootEvent().AddDynamic(this, &ATP_TankPlayer::FxInvisible);
	if (!shieldComponent)return;
	shieldComponent->OnShieldChange().AddDynamic(this,&ATP_TankPlayer::UpdateMaterialShield);
	springArm->bUsePawnControlRotation = false;
	shieldComponent->OnShieldDestroy().AddDynamic(this, &ATP_TankPlayer::TextShield);
	healthComponent->OnMidLife().AddDynamic(this, &ATP_TankPlayer::TextHealth);
	
	ATP_HUD* _tempoHud = Cast<ATP_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!_tempoHud)return;
	dialogueUI = _tempoHud->GetDialogueUI();
	if (!sphereComponent)return;
	sphereComponent->SetWorldScale3D(FVector(0, 0, 0));
}
void ATP_TankPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RayCastToNavMesh();
	OpacityChange();
}
void ATP_TankPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis(moveVertical,this,&ATP_TankPlayer::MoveVertical);
	InputComponent->BindAxis(moveHorizontal,this,&ATP_TankPlayer::MoveHorizontal);
	InputComponent->BindAxis(moveMouseX,this,&ATP_TankPlayer::MoveMouseX);
	InputComponent->BindAxis(moveMouseY,this,&ATP_TankPlayer::MoveMouseY);
	InputComponent->BindAction(shootGatling, EInputEvent::IE_Pressed, shootMinigunSystem, &UTP_ShootMinigunSystemComponent::ActivateShoot);
	InputComponent->BindAction(shootGatling, EInputEvent::IE_Released, shootMinigunSystem, &UTP_ShootMinigunSystemComponent::DesactivateShoot);
	InputComponent->BindAction(shootPlasma, EInputEvent::IE_Pressed, shootPlasmaSystem, &UTP_ShootPlasmaComponent::ActivateShoot);
	InputComponent->BindAction(shootPlasma, EInputEvent::IE_Released, shootPlasmaSystem, &UTP_ShootPlasmaComponent::DesactivateShoot);
	InputComponent->BindAction(shootNuke, EInputEvent::IE_Pressed, shootNukeSystem, &UTP_ShootNukeComponent::ActivateShoot);
	InputComponent->BindAction(dash, EInputEvent::IE_Pressed, dashComponent, &UTP_DashComponent::Dash);
	InputComponent->BindAction(jumpName, EInputEvent::IE_Pressed, this, &ATP_TankPlayer::Jump);
	InputComponent->BindAction(pause, EInputEvent::IE_Pressed, this, &ATP_TankPlayer::PauseWidget).bExecuteWhenPaused = true;;
}

void ATP_TankPlayer::ActivatePlayer()
{
	isActive = true;
	shootMinigunSystem->SetIsActivated(true);
	shootPlasmaSystem->SetIsActivated(true);
	shootNukeSystem->SetIsActivated(true);
}

void ATP_TankPlayer::DesactivatePlayer()
{
	shootMinigunSystem->DesactivateShoot();
	shootPlasmaSystem->DesactivateShoot();
	shootNukeSystem->DesactivateShoot();
	shootMinigunSystem->SetIsActivated(false);
	shootPlasmaSystem->SetIsActivated(false);
	shootNukeSystem->SetIsActivated(false);
	isActive = false;
}
void ATP_TankPlayer::UpdateMaterialShield(float _currentShield,float _maxShield)
{
	float _ratioShield = _currentShield / _maxShield;
	float _ratioGlow = (1 - _ratioShield) * 100;
	if (!materialMeshTankBody)return;
	materialMeshTankBody->SetScalarParameterValue("CracksFresnelPower", _ratioShield);
	materialMeshTankBody->SetScalarParameterValue("GlowIntensityCracks", _ratioGlow);
	if (!materialMeshTurret)return;
	materialMeshTurret->SetScalarParameterValue("CracksFresnelPower", _ratioShield);
	materialMeshTurret->SetScalarParameterValue("GlowIntensityCracks", _ratioGlow);
	if (!materialMeshBaseTurret)return;
	materialMeshBaseTurret->SetScalarParameterValue("CracksFresnelPower", _ratioShield);
	materialMeshBaseTurret->SetScalarParameterValue("GlowIntensityCracks", _ratioGlow);
	UpdateMaterialShieldOnDestroy(_currentShield);
}
void ATP_TankPlayer::UpdateMaterialShieldOnDestroy(float _currentShield)
{
	if (_currentShield > 0)
	{
		materialMeshTankBody->SetScalarParameterValue("BouclierOnOFf", 1);
		materialMeshTurret->SetScalarParameterValue("BouclierOnOFf", 1);
		materialMeshBaseTurret->SetScalarParameterValue("BouclierOnOFf", 1);
	}
	else
	{
		materialMeshTankBody->SetScalarParameterValue("BouclierOnOFf", 0);
		materialMeshTurret->SetScalarParameterValue("BouclierOnOFf", 0);
		materialMeshBaseTurret->SetScalarParameterValue("BouclierOnOFf", 0);
	}
}

void ATP_TankPlayer::FxVisible()
{
	shootFXLeft->Activate(true);
	shootFXRight->Activate(true);
	shootFXRight1->Activate(true);
	shootFXLeft->Activate(true);
	shootFXLeft1->Activate(true);
	douilleEjectLeft->Activate(true);
	douilleEjectRight->Activate(true);
}
void ATP_TankPlayer::FxVisibleWithoutReset()
{
	shootFXLeft->Activate();
	shootFXRight->Activate();
	shootFXRight1->Activate();
	shootFXLeft->Activate();
	shootFXLeft1->Activate();
	douilleEjectLeft->Activate();
	douilleEjectRight->Activate();
}
void ATP_TankPlayer::FxInvisible()
{
	shootFXRight->Deactivate();
	shootFXRight1->Deactivate();
	shootFXLeft->Deactivate();
	shootFXLeft1->Deactivate();
	douilleEjectLeft->Deactivate();
	douilleEjectRight->Deactivate();
}

void ATP_TankPlayer::MoveVertical(float _axis)
{
	UWorld* _world = GetWorld();
	if (!_world || !isActive)return;
	AddMovementInput(GetActorForwardVector(),_axis * moveSpeedForward * _world->DeltaTimeSeconds);
	if (!dashComponent)return;
	dashComponent->vertical = _axis;
	vertical = _axis;
	onMove.Broadcast();
}
void ATP_TankPlayer::MoveHorizontal(float _axis)
{
	UWorld* _world = GetWorld();
	if (!_world || !isActive)return;
	AddMovementInput(GetActorRightVector(), _axis * moveSpeedSide * _world->DeltaTimeSeconds);
	if (!dashComponent)return;
	dashComponent->horizontal = _axis;
	horizontal = _axis;
	onMove.Broadcast();
}
void ATP_TankPlayer::MoveMouseX(float _axis)
{
	UWorld* _world = GetWorld();
	if (!_world || !isActive)return;
	float delta = _world->DeltaTimeSeconds;
	AddControllerYawInput(_axis * rotationSpeedSide * delta);
	meshBaseTurret->SetWorldRotation(FRotator(meshBaseTurret->GetComponentRotation().Pitch, meshTurret->GetComponentRotation().Yaw, meshBaseTurret->GetComponentRotation().Roll));
	meshTankBody->AddWorldRotation(FRotator(meshTankBody->GetComponentRotation().Pitch, (_axis * rotationSpeedSide * delta *-amplitudeRotationBase), meshTankBody->GetComponentRotation().Roll));
}
void ATP_TankPlayer::MoveMouseY(float _axis)
{
	UWorld* _world = GetWorld();
	if (!_world || !isActive)return;
	float delta = _world->DeltaTimeSeconds;
	if (!camera)return;
	FRotator _cameraRotation = camera->GetComponentRotation();
	AddControllerPitchInput(_axis * rotationSpeedUp * delta);
	if (!meshTurret)return;
	if (_cameraRotation.Pitch <= minRotateTurret)
		meshTurret->SetWorldRotation(FRotator(minRotateTurret, _cameraRotation.Yaw, 0));
	else if (_cameraRotation.Pitch >= maxRotateTurret)
		meshTurret->SetWorldRotation(FRotator(maxRotateTurret, _cameraRotation.Yaw, 0));
	else
		meshTurret->SetWorldRotation(FRotator(_cameraRotation));

	FRotator _viewRota = GetViewRotation();
	_viewRota.Pitch = _viewRota.Pitch > 180 ? _viewRota.Pitch - 360 : _viewRota.Pitch;
	_viewRota.Pitch = FMath::Clamp(_viewRota.Pitch, minRotateCamera, maxRotateCamera);
	springArm->SetRelativeRotation(FRotator(_viewRota.Pitch, 0, 0));
	GetController()->SetControlRotation(_viewRota);
}

void ATP_TankPlayer::TextShield()
{
	if (!dialogueUI)
	{
		ATP_HUD* _hud = Cast<ATP_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (!_hud)return;
		dialogueUI = _hud->GetDialogueUI();
	}

	int _len = shieldText.Num();
	if (_len == 0)return;
	int _rand = FMath::RandRange(0, _len-1);
	if (!dialogueUI)return;
	dialogueUI->SetSpeechText(shieldText[_rand]);
}
void ATP_TankPlayer::TextHealth(bool nop)
{
	if (!dialogueUI)
	{
		ATP_HUD* _hud = Cast<ATP_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (!_hud)return;
		dialogueUI = _hud->GetDialogueUI();
	}

	int _len = healthText.Num();
	if (_len == 0)return;
	int _rand = FMath::RandRange(0, _len-1);	
	if (!dialogueUI)return;
	dialogueUI->SetSpeechText(healthText[_rand]);
}

void ATP_TankPlayer::ChangeRaycastRange()
{
	backupDistanceToGround = distanceToGround;
	distanceToGround = 30000;
}
void ATP_TankPlayer::PauseWidget()
{
	
	if (pauseWidget || optionWidget || controlWidget)
	{
		if (controlWidget)
		{
			controlWidget->RemoveFromParent();
			if (!widgetOnPause)return;
			pauseWidget = UWidgetBlueprintLibrary::Create(GetWorld(), widgetOnPause, GetWorld()->GetFirstPlayerController());
			if (!pauseWidget)return;
			pauseWidget->AddToViewport();
			controlWidget = optionWidget = nullptr;
			return;
		}

		UTP_OptionWidget* _option = Cast<UTP_OptionWidget>(optionWidget);
		if (!_option)return;
		_option->ReturnKey();
		controlWidget = optionWidget = nullptr;
		return;
	}

	if (!widgetOnPause)return;
	pauseWidget = UWidgetBlueprintLibrary::Create(GetWorld(), widgetOnPause, GetWorld()->GetFirstPlayerController());
	if (!pauseWidget)return;
	pauseWidget->AddToViewport();
	
}
void ATP_TankPlayer::RayCastToNavMesh()
{
	TArray<AActor*> _toIgnor;
	FHitResult _target;
	_toIgnor.AddUnique(this);

	UWorld* _world = GetWorld();
	if (!_world)return;
	FVector _entityLocation = GetActorLocation();

	UKismetSystemLibrary::LineTraceSingle(_world,
		_entityLocation,
		_entityLocation + FVector::ZAxisVector * -distanceToGround,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		_toIgnor,
		EDrawDebugTrace::None,
		_target,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1);

	

	if (!_target.bBlockingHit)
	{
		onPlayerFaling.Broadcast();
	}
	else
	{
		onNavMeshHit.Broadcast(_target.ImpactPoint);
		if (_target.Distance <= backupDistanceToGround && GetCharacterMovement()->Velocity.Z < 0 )
			distanceToGround = backupDistanceToGround;
	}
		
}

void ATP_TankPlayer::ActorDeath(AActor* _deadActor)
{
	Destroy();
	if (!widgetOnDeath)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), widgetOnDeath, GetWorld()->GetFirstPlayerController());
	if (!_temp)return;
	_temp->AddToViewport();
}

void ATP_TankPlayer::OpacityChange()
{
	if (springArm->IsCollisionFixApplied())
	{
		float _cameraLoc = (camera->GetComponentLocation() - springArm->GetComponentLocation()).Size();
		float translucent = _cameraLoc / springArm->TargetArmLength;
		translucent = translucent < translucentMaxCap ? translucentMaxSet : translucent;
		translucent = translucent > translucentMinCap ? translucentMinSet : translucent;

		if (!materialMeshTankBody || !materialMeshTurret || !materialMeshBaseTurret)return;
		materialMeshTankBody->SetScalarParameterValue("opacity", translucent);
		materialMeshTurret->SetScalarParameterValue("opacity", translucent);
		materialMeshBaseTurret->SetScalarParameterValue("opacity", translucent);
		isOpacityChanged = true;
	}
	else if(isOpacityChanged)
	{
		materialMeshTankBody->SetScalarParameterValue("opacity", 1);
		materialMeshTurret->SetScalarParameterValue("opacity", 1);
		materialMeshBaseTurret->SetScalarParameterValue("opacity", 1);
		isOpacityChanged = false;
	}
}

void ATP_TankPlayer::Jump()
{
	if (!isJumpActivated || !isActive)return;
	Super::Jump();
}