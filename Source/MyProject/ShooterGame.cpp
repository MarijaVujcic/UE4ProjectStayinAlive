// Fill out your copyright notice in the Description page of Project Settings.
#include "ShooterGame.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "CrosshairHUD.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "Misc/DefaultValueHelper.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime\Core\Public\HAL\PlatformFilemanager.h>

AShooterGame::AShooterGame()
{
	this->CurrentLevelNumber = 0;
	this->GameInfoFile = "GameInfo.txt";
	
}
// SMISLIT ZA LOAD COMPLETED LEVELE ??? KAKO LOADAT, MALO PROC KROZ KOD I OPTIMIZIRAT GA I HUD DA SE STVORI SAMO U ODOBRANIM?
// Malo bolje ovo parsiranje
/*
* BeginPlay - called when the game begin (when object of this class is made), sets informations of game, and load next
* level to be played
*/
void AShooterGame::BeginPlay()
{
	Super::BeginPlay();
	bool GameCompleted = false;
	if(this->GameLevels.Num() == 0)
	{
		// pozvati neki error widget ILI TO U BLUEPRINTU
	}

    if (FPaths::FileExists(FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + this->GameInfoFile))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("READ")));

		GameCompleted = this->ReadGameInfo();
		int32 l = this->ReadTotalScore();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Begin %d"), l));

	}
	else
	{
		this->WriteGameInfo("");
	}
	

	if (!GameCompleted)
	{

		UGameplayStatics::OpenLevel(this, FName(*this->EndGameMap, true));
	}
	else if(! GetWorld()->GetMapName().Equals(this->GameLevels[this->CurrentLevelNumber])){


		UGameplayStatics::OpenLevel(this, FName(*this->GameLevels[this->CurrentLevelNumber]), true);
	}
	HUDClass = ACrosshairHUD::StaticClass();

}

/*
* LoadNextLevel Checks if nect level exist in GameLevels array, if does, it sets next level as current and return true, otherwise return false
*/
bool AShooterGame::LoadNextLevel()
{
	if (this->GameLevels.Contains(this->NextLevelName))
	{
		this->CurrentLevelNumber=this->GameLevels.Find(this->NextLevelName);
		this->CurrentLevelName = this->NextLevelName;
		return true;
	}
	return false;
}

/*
* Method mostly called in blueprint for settng score and status to certain level (status is hardcoded as 'COMPLETED' for now)
* @param FString - Level which is completed
* @param int32 - score accomplished in that level
*/
void AShooterGame::LevelComplete(FString LevelWhichIsCompleted, int32 Score)
{
	WriteGameInfo(LevelWhichIsCompleted, Score);
}

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

/********************************/
/**** GameInfoFile methods ****/

/*
*
* ReadGameInfo - (if GameInfoFile exist) parse GameInfoFile and sets which level need to open while starting the game
* returns false in case game is completed(there are no more levels to read), true otherwise
*
*/
bool AShooterGame::ReadGameInfo()
{
	FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + this->GameInfoFile;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("READ")));

	FFileHelper::LoadFileToString(this->ReadValue,*(FilePath));
	int32 RetIndex = 0;
	bool CompletedExist;
	FString MakeNew;
	CompletedExist = this->ReadValue.Contains("COMPLETED");
	if (CompletedExist)
	{
		RetIndex = this->ReadValue.Find("COMPLETED", ESearchCase::CaseSensitive, ESearchDir::FromEnd);
		RetIndex += 10;
		if (RetIndex+1 >= this->ReadValue.Len())
		{
			return false;
		}
		while ( this->ReadValue[RetIndex] != ' ')
		{
			MakeNew.AppendChar(this->ReadValue[RetIndex]);
			RetIndex += 1;
		}
		this->NextLevelName = MakeNew;
		return LoadNextLevel();
	}
	return true;
}

/*
*
* WriteGameInfo - (if GameInfoFile doesn't exist, first time creation of file) create and write levels informations in GameInfoFile
*
*/
void AShooterGame::WriteGameInfo(FString LevelWhichIsCompleted, int32 Score)
{
	FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + this->GameInfoFile;
	if (LevelWhichIsCompleted != "")
	{
		IPlatformFile& Reading = FPlatformFileManager::Get().GetPlatformFile();
		Reading.DeleteFile(*FilePath);
	}

	FString FileHeader = TEXT("*****************************************************\n");
	FileHeader += TEXT("***************   GAME INFO FILE   ******************\n");
	FileHeader += TEXT("*****************************************************\n");
	FileHeader += TEXT("LEVEL_NAME SCORE STATUS\n\n");
	FString FileContent;
	for (int i = 0; i < this->GameLevels.Num(); i++)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("FOR")));
		if (LevelWhichIsCompleted.Equals(this->GameLevels[i]))
		{
			FileContent +=   this->GameLevels[i] + ' ' + FString::FromInt(Score) +" COMPLETED\n";
		}
		else
		{
			FileContent += this->GameLevels[i] + " 0 \n";
		}
		
	}

	FFileHelper::SaveStringToFile(FileHeader, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);

}

/*
* Reads total score of completed levels
* @return int32 total game score
*/
int32 AShooterGame::ReadTotalScore()
{
	if (! FPaths::FileExists(FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + this->GameInfoFile) || this->GameLevels.Num()==0 /*|| !this->ReadValue.Contains("COMPLETED")*/)
	{
		return 0;
	}
	int32 ReturnScore = 0;
	int32 FindCompleted = 0;
	FString Number;

	for (int i = 0; i <= this->GameLevels.Num()-1; i++)
	{
		FindCompleted = this->ReadValue.Find(this->GameLevels[i]);
		FindCompleted += this->GameLevels[i].Len() + 1;

		while( this->ReadValue[FindCompleted] != '\n' )
		{
			if (this->ReadValue[FindCompleted] == ' ')
			{
				break;
			}
			Number.AppendChar(this->ReadValue[FindCompleted]);

			FindCompleted+=1;
		}
		ReturnScore += FCString::Atoi(*Number);
		Number.Empty();
	}
	return ReturnScore;
}
