// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Input/CommonInputMode.h"

#include "InputBindingActionWidget.generated.h"

enum class ECommonInputType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionCompleteEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionProgressEvent, float, HeldPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputMethodChangedEvent, ECommonInputType, CurrentInputType);


UCLASS()
class COMMONUIEXTENSION_API UInputBindingActionWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;	
	virtual void SetIsEnabled(bool bInIsEnabled) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Input | Getters")
	bool GetInputAction(FDataTableRowHandle& InputActionRow) const;

	UFUNCTION(BlueprintCallable, Category = "Input | Setters")
	void SetInputAction(const FDataTableRowHandle& InputActionRow);

	UFUNCTION(BlueprintCallable, Category = "Input | Getters")
	FText GetDisplayText() const;

	UFUNCTION(BlueprintCallable, Category = "Input | Getters")
	bool IsHeldAction() const;

	UFUNCTION(BlueprintCallable, Category = "Input | Getters")
	void SetHoldTime(float HoldTime, bool& Result);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void OnInputMethodChanged(ECommonInputType CurrentInputType);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void OnActionProgress(float HeldPercent);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void OnActionComplete();

	UFUNCTION()
	virtual void NativeInputMethodChanged(ECommonInputType CurrentInputType);

	UFUNCTION()
	virtual void NativeOnActionProgress(float HeldPercent);

	UFUNCTION()
	virtual void NativeOnActionComplete();

private:

	void OverwriteHoldTime();
	bool IsHoldActrive() const;

	void UpdateInputActionWidgetVisibility();
	void UpdateInputActionWidget();

	void BindTriggeringInputAction();
	void UnbindTriggeringInputAction();
	void BindInputMethodChangedDelegate();
	void UnbindInputMethodChangedDelegate();


public:
	// CommonButtonBase Ç≈Ç‡ñ¢égópÅH
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (ExposeOnSpawn = true))
	//int32 InputPriority;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (ExposeOnSpawn = true, RowType = CommonInputActionDataBase))
	FDataTableRowHandle TriggeringInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (ExposeOnSpawn = true))
	uint8 bHideInputActionWithKeyboard : 1;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, AdvancedDisplay, meta = (ClampMin = 0.0f))
	float HoldTimeOverride = 0.0f;

	UPROPERTY(EditAnywhere, Category = Input, AdvancedDisplay)
	bool bIsPersistentBinding = false;

	UPROPERTY(EditAnywhere, Category = Input, AdvancedDisplay)
	ECommonInputMode InputModeOverride = ECommonInputMode::Game;

	FUIActionBindingHandle TriggeringBindingHandle;

	UPROPERTY(BlueprintReadOnly, Category = Input, meta = (BindWidget, OptionalWidget = true, AllowPrivateAccess = true))
	class UCommonActionWidget* InputActionWidget;

	
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnActionProgressEvent OnActionProgressEvent;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnActionCompleteEvent OnActionCompleteEvent;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnInputMethodChangedEvent OnInputMethodChangedEvent;

};
