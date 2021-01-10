// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
/// Controller Button names sourced from
/// https://gist.github.com/palmerj/586375bcc5bc83ccdaf00c6f5f863e86
/// Can be used as a variable type in Blueprints.
UENUM(BlueprintType)
enum class EControllerButton : uint8
{
	A, B, X, Y,
    LeftTrigger, RightTrigger,
    LeftBumper, RightBumper,
    LeftStickButton, RightStickButton,
    DPadUp, DPadDown, DPadLeft, DPadRight,
    Start,
    Back
};
