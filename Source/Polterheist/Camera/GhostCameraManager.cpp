// Fill out your copyright notice in the Description page of Project Settings.


#include "Polterheist/Camera/GhostCameraManager.h"

AGhostCameraManager::AGhostCameraManager() : APlayerCameraManager()
{
    ViewPitchMax = 80.0f;
    ViewPitchMin = -45.0f;
}
