#include "TP_InputManagerWidget.h"

void UTP_InputManagerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//SetKeyboardInvisible();
	SetControllerInvisible();
	Init();
	//GetWorld()->GetTimerManager().SetTimer(timer, this, &UTP_InputManagerWidget::LinkAll, 0.1, false);
	ShootMinigun->OnKeySelected.AddDynamic(this, &UTP_InputManagerWidget::ChangeActionKeyShootM);
	ShootPlasma->OnKeySelected.AddDynamic(this, &UTP_InputManagerWidget::ChangeActionKeyShootP);
	ShootNuke->OnKeySelected.AddDynamic(this, &UTP_InputManagerWidget::ChangeActionKeyShootN);
	Jump->OnKeySelected.AddDynamic(this, &UTP_InputManagerWidget::ChangeActionKeyJump);
	Dash->OnKeySelected.AddDynamic(this, &UTP_InputManagerWidget::ChangeActionKeyDash);
	MoveForward->OnKeySelected.AddDynamic(this, &UTP_InputManagerWidget::ChangeAxiskeyForward);
	MoveBackward->OnKeySelected.AddDynamic(this, &UTP_InputManagerWidget::ChangeAxiskeyBackward);
	MoveRight->OnKeySelected.AddDynamic(this, &UTP_InputManagerWidget::ChangeAxiskeyRight);
	MoveLeft->OnKeySelected.AddDynamic(this, &UTP_InputManagerWidget::ChangeAxiskeyLeft);
	mouseSensitivity->OnMouseCaptureEnd.AddDynamic(this, &UTP_InputManagerWidget::ChangeSensi);
	Controller_B->OnClicked.AddDynamic(this, &UTP_InputManagerWidget::SetKeyboardInvisible);
	Controller_B->OnClicked.AddDynamic(this, &UTP_InputManagerWidget::SetControllerVisible);
	Keyboard_B->OnClicked.AddDynamic(this, &UTP_InputManagerWidget::SetControllerInvisible);
	Keyboard_B->OnClicked.AddDynamic(this, &UTP_InputManagerWidget::SetKeyboardControlVisible);
	
}

void UTP_InputManagerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

//initialise toutes les varaibles et tableau de touches
void UTP_InputManagerWidget::Init()
{
	ourSettings = UInputSettings::GetInputSettings();
	if (!ourSettings)return;
	ourSettings->GetActionMappingByName("Shoot_Gatling", shoot_Minigun);
	ourSettings->GetActionMappingByName("Shoot_Plasma", shoot_Plasma);
	ourSettings->GetActionMappingByName("Shoot_Nuke", shoot_Nuke);
	ourSettings->GetActionMappingByName("Dash", dash_);
	ourSettings->GetActionMappingByName("Jump", jump_);
	ourSettings->GetAxisMappingByName("Move_Vertical", moveFront);
	ourSettings->GetAxisMappingByName("Move_Horizontale", moveSideway);
	ourSettings->GetAxisMappingByName("Move_Mouse_X", mouseX);
	ourSettings->GetAxisMappingByName("Move_Mouse_Y", mouseY);



#pragma region AllRegisteredKeys

#pragma region ShootKeys
	FInputChord _tempo;
	_tempo.Key = shoot_Minigun[0].Key;
	allBindedKeys.Add(shoot_Minigun[0].Key);
	ShootMinigun->SetSelectedKey(_tempo);

	_tempo.Key = shoot_Plasma[0].Key;
	allBindedKeys.Add(shoot_Plasma[0].Key);
	ShootPlasma->SetSelectedKey(_tempo);

	_tempo.Key = shoot_Nuke[0].Key;
	allBindedKeys.Add(shoot_Nuke[0].Key);
	ShootNuke->SetSelectedKey(_tempo);

#pragma endregion

#pragma region Dash&JumpKeys
	_tempo.Key = dash_[0].Key;
	allBindedKeys.Add(dash_[0].Key);
	Dash->SetSelectedKey(_tempo);
	_tempo.Key = jump_[0].Key;
	allBindedKeys.Add(jump_[0].Key);
	Jump->SetSelectedKey(_tempo);

#pragma endregion 

#pragma region DirectionKeys
	allKeys.Add("FrontForwardC", moveFront[2]);
	allKeys.Add("FrontBackwardC", moveFront[1]);
	allKeys.Add("FrontM", moveFront[0]);
	_tempo.Key = allKeys["FrontForwardC"].Key;
	MoveForward->SetSelectedKey(_tempo);
	_tempo.Key = allKeys["FrontBackwardC"].Key;
	MoveBackward->SetSelectedKey(_tempo);
	allBindedKeys.Add(allKeys["FrontForwardC"].Key);
	allBindedKeys.Add(allKeys["FrontBackwardC"].Key);

	allKeys.Add("SidewayRightC", moveSideway[2]);
	allKeys.Add("SidewayLeftC", moveSideway[1]);
	allKeys.Add("SidewayM", moveSideway[0]);
	_tempo.Key = allKeys["SidewayRightC"].Key;
	MoveRight->SetSelectedKey(_tempo);
	_tempo.Key = allKeys["SidewayLeftC"].Key;
	MoveLeft->SetSelectedKey(_tempo);
	allBindedKeys.Add(allKeys["SidewayRightC"].Key);
	allBindedKeys.Add(allKeys["SidewayLeftC"].Key);

	allKeys.Add("MouseXM", mouseX[0]);
	allKeys.Add("MouseXC", mouseX[1]);

	allKeys.Add("MouseYM", mouseY[0]);
	allKeys.Add("MouseYC", mouseY[1]);

#pragma endregion
#pragma endregion
	mouseSensitivity->SetValue(allKeys["MouseXC"].Scale); // Changed MouseX[0].scale by allKeys["MouseXC"].Scale, From Line 47 move to here (Line 106)
	mouseSensitivity->SetMaxValue(max);
	mouseSensitivity->SetMinValue(min);
}

void UTP_InputManagerWidget::ChangeSensi()
{
	ourSettings->RemoveAxisMapping(allKeys["MouseXM"]);
	ourSettings->RemoveAxisMapping(allKeys["MouseXC"]);
	FInputAxisKeyMapping _tempo;
	_tempo.AxisName = "Move_Mouse_X";

	if (allKeys["MouseXC"].Scale < 0)
		allKeys["MouseXC"].Scale = -mouseSensitivity->GetValue();
	else
		allKeys["MouseXC"].Scale = mouseSensitivity->GetValue();
	_tempo.Scale = allKeys["MouseXC"].Scale;
	_tempo.Key = allKeys["MouseXC"].Key;
	ourSettings->AddAxisMapping(_tempo, true);
	_tempo.Scale = allKeys["MouseXM"].Scale;
	_tempo.Key = allKeys["MouseXM"].Key;
	ourSettings->AddAxisMapping(_tempo, true);



	ourSettings->RemoveAxisMapping(allKeys["MouseYM"]);
	ourSettings->RemoveAxisMapping(allKeys["MouseYC"]);
	_tempo.AxisName = "Move_Mouse_Y";

	if (allKeys["MouseYC"].Scale < 0)
		allKeys["MouseYC"].Scale = -mouseSensitivity->GetValue();
	else
		allKeys["MouseYC"].Scale = mouseSensitivity->GetValue();
	_tempo.Scale = allKeys["MouseYC"].Scale;
	_tempo.Key = allKeys["MouseYC"].Key;
	ourSettings->AddAxisMapping(_tempo, true);
	_tempo.Scale = allKeys["MouseYM"].Scale;
	_tempo.Key = allKeys["MouseYM"].Key;
	ourSettings->AddAxisMapping(_tempo, true);
	
	SaveConfig();
}

#pragma region ChangeActionsKeys
//fonction generale qui est appeler dans toute les sous fonction pour les action mapping
void UTP_InputManagerWidget::ChangeKey(FInputChord _chord, FName _actionName, TArray<FInputActionKeyMapping>& _action, UInputKeySelector* _selectorName, int _index)
{
	FInputActionKeyMapping _tempo;
	_tempo.ActionName = _actionName;
	if (!CheckKeyFree(_chord.Key))
	{
		_selectorName->SetSelectedKey(_action[_index].Key);
		return;
	}

	for (size_t i = 0; i < _action.Num(); i++)
	{
		ourSettings->RemoveActionMapping(_action[0]);
	}

	for (size_t i = 0; i < _action.Num(); i++)
	{
		if (i == _index)
		{
			allBindedKeys.Remove(_action[i].Key);
			_action[i].Key = _chord.Key;
			allBindedKeys.Add(_action[i].Key);
		}
		_tempo.Key = _action[i].Key;
		ourSettings->AddActionMapping(_tempo, true);
	}
	ourSettings->SaveConfig();
}

void UTP_InputManagerWidget::ChangeActionKeyDash(FInputChord _chord)
{
	ChangeKey(_chord, "Dash", dash_, Dash, 0);
}

void UTP_InputManagerWidget::ChangeActionKeyJump(FInputChord _chord)
{
	ChangeKey(_chord, "Jump", jump_, Jump, 0);
}

void UTP_InputManagerWidget::ChangeActionKeyShootM(FInputChord _chord)
{
	ChangeKey(_chord, "Shoot_Gatling", shoot_Minigun, ShootMinigun, 0);
}

void UTP_InputManagerWidget::ChangeActionKeyShootP(FInputChord _chord)
{
	ChangeKey(_chord, "Shoot_Plasma", shoot_Plasma, ShootPlasma, 0);
}

void UTP_InputManagerWidget::ChangeActionKeyShootN(FInputChord _chord)
{
	ChangeKey(_chord, "Shoot_Nuke", shoot_Nuke, ShootNuke, 0);
}
#pragma endregion

//verifie si la touche qu'on veut attribuer ne l'est pas deja autre part
bool UTP_InputManagerWidget::CheckKeyFree(FKey _myKey)
{
	for (size_t i = 0; i < allBindedKeys.Num(); i++)
	{
		if (allBindedKeys[i] == _myKey)
			return false;
	}
	return true;
}

#pragma region ChangeDirectionKeys
//permet de changer la touche pour avancer 
void UTP_InputManagerWidget::ChangeAxiskeyForward(FInputChord _chord)
{
	if (!CheckKeyFree(_chord.Key))
	{
		MoveForward->SetSelectedKey(allKeys["FrontForwardC"].Key);
		return;
	}
	ourSettings->RemoveAxisMapping(allKeys["FrontForwardC"]);
	ourSettings->RemoveAxisMapping(allKeys["FrontBackwardC"]);
	ourSettings->RemoveAxisMapping(allKeys["FrontM"]);
	FInputAxisKeyMapping _tempo;
	_tempo.AxisName = "Move_Vertical";

	allBindedKeys.Remove(allKeys["FrontForwardC"].Key);
	allKeys["FrontForwardC"].Key = _chord.Key;
	allBindedKeys.Add(allKeys["FrontForwardC"].Key);

	_tempo.Key = allKeys["FrontForwardC"].Key;
	_tempo.Scale = allKeys["FrontForwardC"].Scale;
	ourSettings->AddAxisMapping(_tempo, true);
	_tempo.Key = allKeys["FrontBackwardC"].Key;
	_tempo.Scale = allKeys["FrontBackwardC"].Scale;
	ourSettings->AddAxisMapping(_tempo, true);
	_tempo.Key = allKeys["FrontM"].Key;
	_tempo.Scale = allKeys["FrontM"].Scale;
	ourSettings->AddAxisMapping(_tempo, true);

	ourSettings->SaveConfig();
}

//permet de changer la touche pour reculer
void UTP_InputManagerWidget::ChangeAxiskeyBackward(FInputChord _chord)
{
	if (!CheckKeyFree(_chord.Key))
	{
		MoveForward->SetSelectedKey(allKeys["FrontBackwardC"].Key);
		return;
	}
	ourSettings->RemoveAxisMapping(allKeys["FrontForwardC"]);
	ourSettings->RemoveAxisMapping(allKeys["FrontBackwardC"]);
	ourSettings->RemoveAxisMapping(allKeys["FrontM"]);
	FInputAxisKeyMapping _tempo;
	_tempo.AxisName = "Move_Vertical";

	allBindedKeys.Remove(allKeys["FrontBackwardC"].Key);
	allKeys["FrontBackwardC"].Key = _chord.Key;
	allBindedKeys.Add(allKeys["FrontBackwardC"].Key);

	_tempo.Key = allKeys["FrontForwardC"].Key;
	_tempo.Scale = allKeys["FrontForwardC"].Scale;
	ourSettings->AddAxisMapping(_tempo, true);
	_tempo.Key = allKeys["FrontBackwardC"].Key;
	_tempo.Scale = allKeys["FrontBackwardC"].Scale;
	ourSettings->AddAxisMapping(_tempo, true);
	_tempo.Key = allKeys["FrontM"].Key;
	_tempo.Scale = allKeys["FrontM"].Scale;
	ourSettings->AddAxisMapping(_tempo, true);


	ourSettings->SaveConfig();
}

//permet de changer la touche pour aller a droite
void UTP_InputManagerWidget::ChangeAxiskeyRight(FInputChord _chord)
{
	if (!CheckKeyFree(_chord.Key))
	{
		MoveRight->SetSelectedKey(allKeys["SidewayRightC"].Key);
		return;
	}

	ourSettings->RemoveAxisMapping(allKeys["SidewayRightC"]);
	ourSettings->RemoveAxisMapping(allKeys["SidewayLeftC"]);
	ourSettings->RemoveAxisMapping(allKeys["SidewayM"]);
	FInputAxisKeyMapping _tempo;
	_tempo.AxisName = "Move_Horizontale";

	allBindedKeys.Remove(allKeys["SidewayRightC"].Key);
	allKeys["SidewayRightC"].Key = _chord.Key;
	allBindedKeys.Add(allKeys["SidewayRightC"].Key);


	_tempo.Key = allKeys["SidewayRightC"].Key;
	_tempo.Scale = allKeys["SidewayRightC"].Scale;
	ourSettings->AddAxisMapping(_tempo, true);
	_tempo.Key = allKeys["SidewayLeftC"].Key;
	_tempo.Scale = allKeys["SidewayLeftC"].Scale;
	ourSettings->AddAxisMapping(_tempo, true);
	_tempo.Key = allKeys["SidewayM"].Key;
	_tempo.Scale = allKeys["SidewayM"].Scale;
	ourSettings->AddAxisMapping(_tempo, true);

	ourSettings->SaveConfig();
}

//permet de changer la touche pour aller a gauche 
void UTP_InputManagerWidget::ChangeAxiskeyLeft(FInputChord _chord)
{
	if (!CheckKeyFree(_chord.Key))
	{
		MoveLeft->SetSelectedKey(allKeys["SidewayLeftC"].Key);
		return;
	}

	ourSettings->RemoveAxisMapping(allKeys["SidewayRightC"]);
	ourSettings->RemoveAxisMapping(allKeys["SidewayLeftC"]);
	ourSettings->RemoveAxisMapping(allKeys["SidewayM"]);
	FInputAxisKeyMapping _tempo;
	_tempo.AxisName = "Move_Horizontale";

	allBindedKeys.Remove(allKeys["SidewayLeftC"].Key);
	allKeys["SidewayLeftC"].Key = _chord.Key;
	allBindedKeys.Add(allKeys["SidewayLeftC"].Key);

	_tempo.Key = allKeys["SidewayRightC"].Key;
	_tempo.Scale = allKeys["SidewayRightC"].Scale;
	ourSettings->AddAxisMapping(_tempo, true);
	_tempo.Key = allKeys["SidewayLeftC"].Key;
	_tempo.Scale = allKeys["SidewayLeftC"].Scale;
	ourSettings->AddAxisMapping(_tempo, true);
	_tempo.Key = allKeys["SidewayM"].Key;
	_tempo.Scale = allKeys["SidewayM"].Scale;
	ourSettings->AddAxisMapping(_tempo, true);

	ourSettings->SaveConfig();
}
//les fonctions qui rende visible ou invisible tels canvas selon si on veut les touche mannete ou clavier
void UTP_InputManagerWidget::SetKeyboardControlVisible()
{
	Keyboard->SetVisibility(ESlateVisibility::Visible);
}
void UTP_InputManagerWidget::SetControllerVisible()
{
	Controller->SetVisibility(ESlateVisibility::Visible);
}
void UTP_InputManagerWidget::SetKeyboardInvisible()
{
	Keyboard->SetVisibility(ESlateVisibility::Hidden);
}
void UTP_InputManagerWidget::SetControllerInvisible()
{
	Controller->SetVisibility(ESlateVisibility::Hidden);
}
#pragma endregion
