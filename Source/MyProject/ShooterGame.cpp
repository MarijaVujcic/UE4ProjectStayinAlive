// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGame.h"
#include "CrosshairHUD.h"
AShooterGame::AShooterGame()
{
	HUDClass = ACrosshairHUD::StaticClass();
}
