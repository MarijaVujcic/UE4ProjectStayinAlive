// Fill out your copyright notice in the Description page of Project Settings.
#include "ShooterGame.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "CrosshairHUD.h"
#include "Engine/Engine.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime\Core\Public\HAL\PlatformFilemanager.h>

AShooterGame::AShooterGame()
{
	HUDClass = ACrosshairHUD::StaticClass();
	this->CurrentLevelNumber = 0; //namistit da cita kasnije iz .txt ili neke vrste datoteke
	this->GameInfoFile = "GameInfo.txt";
}
// implementirat rad sa fileom, updateanje filea ako je level completed
// i malo bolje ove funkcije napisat
void AShooterGame::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("BEGIN PLAY SHOOTER GAME ")));

	if(this->GameLevels.Num() == 0)
	{
		// pozvati neki error widget
	}

    if (FPaths::FileExists(FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + this->GameInfoFile))
	{
		this->ReadGameInfo();
	}
	else
	{
		this->WriteGameInfo();
	}

	if (GetWorld()->GetMapName() != *this->GameLevels[this->CurrentLevelNumber]) {
		UGameplayStatics::OpenLevel(this, FName(*this->GameLevels[this->CurrentLevelNumber]), true);
	}

//	////this->DefaultPlayerController->bShowMouseCursor = true;
//	//if (!this->MainMenuWidgetName)
//	//{
//	//	
//	//}
//	///*UGameplayStatics::OpenLevel(this, FName(*this->MainMenuLevel), true);*/
//	//
//	///*FInputModeUIOnly InputMode;
//	//this->DefaultPlayerController->SetInputMode(InputMode);*/

//
//	////FInputModeGameOnly InputMode;
//	//////ovo sve postavljam u spaceship characteru pa to tamo maknnut
//	////this->DefaultPlayerController = GetWorld()->GetFirstPlayerController();
//	////this->DefaultPlayerController->SetInputMode(InputMode);
//	////this->DefaultPlayerController->bShowMouseCursor = false;
//	////
//	////	CheckLevel();
//	////
//
}

void AShooterGame::LoadNextLevel()
{
	if (this->GameLevels.Contains(this->NextLevelName))
	{
		FName LoadingLevel = FName(*this->NextLevelName);

		UGameplayStatics::OpenLevel(this, LoadingLevel, true);
	}
	else
	{
		if (this->LevelCompleteWidget)
		{
			this->LevelCompleteWidget->RemoveFromParent();
			if (this->DeafultGameCompleteWidget)
			{
				this->GameCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), this->DeafultGameCompleteWidget);
				this->GameCompleteWidget->AddToViewport();
				this->DefaultPlayerController->bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
				this->DefaultPlayerController->SetInputMode(InputMode);
		}
		}
	}
}

void AShooterGame::LevelComplete()
{/*
	if (this->DeafultLevelCompleteWidget)
	{
		this->LevelCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), this->DeafultLevelCompleteWidget);
		this->LevelCompleteWidget->AddToViewport();
	}
	GetWorldTimerManager().SetTimer(this->LevelSwap, this, &AShooterGame::LoadNextLevel, 2.0f, false);*/
}
//
void AShooterGame::EndGame()
{
	FString LevelName = GetWorld()->GetMapName();
	FName LoadingLevel = FName(*LevelName);
   
	UGameplayStatics::OpenLevel(this, LoadingLevel, true);
}
//
void AShooterGame::CheckLevel()
{
	this->CurrentLevelName = GetWorld()->GetMapName();
	this->GameLevels.Find(this->CurrentLevelName, this->CurrentLevelNumber);
	if (this->CurrentLevelNumber <= this->GameLevels.Num() - 1)
	{
		this->NextLevelName = this->GameLevels[this->CurrentLevelNumber + 1];
	}
	else
	{
		this->NextLevelName = "GameCompleted";
	}
}

//////////////////////////////////
///// GameInfo File methods ////
void AShooterGame::ReadGameInfo()
{ 
	// napisat parsinranje tako da procita string

	FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + this->GameInfoFile;
	/*FString ReadValue;*/
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("READ")));
	//IPlatformFile& Reading= FPlatformFileManager::Get().GetPlatformFile();

	//IFileHandle* FileHandle = Reading.OpenRead(*FilePath);
	//if (FileHandle)
	//{
	//	FileHandle->Seek(183);
	//	FileHandle.
	//}
	//FFileHelper::LoadFileToString(ReadValue,*(FilePath));
	int32 RetIndex = 0;
	//FFileHelper::SaveStringToFile(ReadValue, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	for (int i = 0; i < this->GameLevels.Num(); i++)
	{
		
	}
}



// security staviti da ne moze niko brisat ni dodavat ista osim programa
void AShooterGame::WriteGameInfo()
{
	FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + this->GameInfoFile;
	FString FileHeader = TEXT("*****************************************************\n");
	FileHeader += TEXT("***************   GAME INFO FILE   ******************\n");
	FileHeader += TEXT("*****************************************************\n");
	FileHeader += TEXT("LEVEL_NAME SCORE STATUS\n\n");
	FString FileContent;
	for (int i = 0; i < this->GameLevels.Num(); i++)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("FOR")));

		FileContent +=  this->GameLevels[i] + TEXT(" 0  \n");
	}
	FFileHelper::SaveStringToFile(FileHeader, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);

}
