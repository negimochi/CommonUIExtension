// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBindingActionWidget.h"

#ifdef COMMON_UI_PRIVATE_ACCESS
#include "Input/UIActionRouterTypes.h"
#endif // COMMON_UI_PRIVATE_ACCESS


#include "Input/CommonUIInputTypes.h"
#include "CommonActionWidget.h"
#include "CommonActionWidget.h"
#include "ICommonInputModule.h"


void UInputBindingActionWidget::NativeConstruct()
{
	BindTriggeringInputAction();
	BindInputMethodChangedDelegate();
	UpdateInputActionWidget();

	Super::NativeConstruct();
}

void UInputBindingActionWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UnbindTriggeringInputAction();
	UnbindInputMethodChangedDelegate();
}

void UInputBindingActionWidget::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);
	if (InputActionWidget)
	{
		InputActionWidget->SetIsEnabled(bInIsEnabled);
	}
}

void UInputBindingActionWidget::BindInputMethodChangedDelegate()
{
	if (auto* subsystem = GetInputSubsystem(); subsystem != nullptr)
	{
		subsystem->OnInputMethodChangedNative.AddUObject(this, &UInputBindingActionWidget::OnInputMethodChanged);
	}
}

void UInputBindingActionWidget::UnbindInputMethodChangedDelegate()
{
	if (auto* subsystem = GetInputSubsystem(); subsystem != nullptr)
	{
		subsystem->OnInputMethodChangedNative.RemoveAll(this);
	}
}

void UInputBindingActionWidget::BindTriggeringInputAction()
{
	if (TriggeringInputAction.IsNull())
	{
		return;
	}

	if (!TriggeringBindingHandle.IsValid())
	{
		FBindUIActionArgs bindUIActionArgs(TriggeringInputAction, false, 
									FSimpleDelegate::CreateUObject(this, &UInputBindingActionWidget::NativeOnActionComplete));
		bindUIActionArgs.OnHoldActionProgressed.BindUObject(this, &UInputBindingActionWidget::NativeOnActionProgress);
		bindUIActionArgs.bIsPersistent = bIsPersistentBinding;
		bindUIActionArgs.InputMode = InputModeOverride;

		TriggeringBindingHandle = RegisterUIActionBinding(bindUIActionArgs);

#ifdef COMMON_UI_PRIVATE_ACCESS
		if (HoldTimeOverride > 0.f) {
			OverwriteHoldTime();
		}
#endif // COMMON_UI_PRIVATE_ACCESS
	}
}

void UInputBindingActionWidget::OverwriteHoldTime()
{
#ifdef COMMON_UI_PRIVATE_ACCESS
	if (TriggeringBindingHandle.IsValid())
	{
		if (auto binding = FUIActionBinding::FindBinding(TriggeringBindingHandle))
		{
			for (FUIActionKeyMapping& holdMapping : binding->HoldMappings)
			{
				holdMapping.HoldTime = HoldTimeOverride;
			}
		}
	}
#endif // COMMON_UI_PRIVATE_ACCESS
}


void UInputBindingActionWidget::UnbindTriggeringInputAction()
{
	if (TriggeringBindingHandle.IsValid())
	{
		TriggeringBindingHandle.Unregister();
	}
}

bool UInputBindingActionWidget::GetInputAction(FDataTableRowHandle& InputActionRow) const
{
	if (!TriggeringInputAction.IsNull())
	{
		InputActionRow = TriggeringInputAction;
		return true;
	}
	return false;
}

void UInputBindingActionWidget::SetInputAction(const FDataTableRowHandle& InputActionRow)
{
	if (TriggeringInputAction != InputActionRow)
	{
		UnbindTriggeringInputAction();

		TriggeringInputAction = InputActionRow;

		if (!IsDesignTime())
		{
			BindTriggeringInputAction();
		}

		UpdateInputActionWidget();
	}
}

FText UInputBindingActionWidget::GetDisplayText() const
{
	if (InputActionWidget)
	{
		return InputActionWidget->GetDisplayText();
	}
	return FText::GetEmpty();
}

bool UInputBindingActionWidget::IsHeldAction() const
{
	if (InputActionWidget)
	{
		return InputActionWidget->IsHeldAction();
	}
	return false;
}


void UInputBindingActionWidget::SetHoldTime(float HoldTime)
{
#ifdef COMMON_UI_PRIVATE_ACCESS
	if (HoldTime > 0.f)
	{
		UnbindTriggeringInputAction();

		HoldTimeOverride = HoldTime;

		if (!IsDesignTime())
		{
			BindTriggeringInputAction();
		}

		UpdateInputActionWidget();
	}
#endif
}

void UInputBindingActionWidget::NativeOnActionProgress(float HeldPercent)
{
	if (InputActionWidget)
	{
		InputActionWidget->OnActionProgress(HeldPercent);
	}
	OnActionProgress(HeldPercent);
	OnActionProgressEvent.Broadcast(HeldPercent);
}

void UInputBindingActionWidget::NativeOnActionComplete()
{
	if (InputActionWidget)
	{
		InputActionWidget->OnActionComplete();
	}
	OnActionComplete();
	OnActionCompleteEvent.Broadcast();
}

void UInputBindingActionWidget::NativeInputMethodChanged(ECommonInputType CurrentInputType)
{
	UpdateInputActionWidget();
	OnInputMethodChanged(CurrentInputType);
	OnInputMethodChangedEvent.Broadcast(CurrentInputType);
}

void UInputBindingActionWidget::UpdateInputActionWidget()
{
	if (GetGameInstance() && InputActionWidget)
	{
		if (!TriggeringInputAction.IsNull())
		{
			InputActionWidget->SetInputAction(TriggeringInputAction);
		}
		else
		{
			FDataTableRowHandle EmptyStateHandle;
			InputActionWidget->SetInputAction(EmptyStateHandle);
		}

		UpdateInputActionWidgetVisibility();
	}
}

void UInputBindingActionWidget::UpdateInputActionWidgetVisibility()
{
	if (InputActionWidget)
	{
		bool bHidden = false;

		auto* CommonInputSubsystem = GetInputSubsystem();

		if (CommonInputSubsystem != nullptr) 
		{
			bHidden = bHideInputActionWithKeyboard && CommonInputSubsystem->GetCurrentInputType() != ECommonInputType::Gamepad;
		}

		InputActionWidget->SetHidden(bHidden);
	}
}
