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

/*
* Contructor of clas AShooterGame
*/
AShooterGame::AShooterGame()
{
	this->CurrentLevelNumber = 0;
	this->GameInfoFile = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir()) + "GameInfo.txt";
	this->GameCompleted = false;
	PrimaryActorTick.bCanEverTick = true;
}

/*
* BeginPlay - called when the game begin (when object of this class is made), sets informations of game, and load next
* level to be played
*/
void AShooterGame::BeginPlay()
{
	Super::BeginPlay();

	if (FPaths::FileExists(this->GameInfoFile))
	{
		this->GameCompleted = this->ReadGameInfo();
	}
	else
	{
		this->WriteGameInfo();
	}
}

void AShooterGame::StartLevel()
{
	if (this->GameCompleted)
	{
		UGameplayStatics::OpenLevel(this, FName(*this->EndGameMap, true));
	}
	else if (!GetWorld()->GetMapName().Equals(this->GameLevels[this->CurrentLevelNumber]))
	{
		UGameplayStatics::OpenLevel(this, FName(*this->GameLevels[this->CurrentLevelNumber]), true);
	}
}
void AShooterGame::RepeatLevel(FString LevelName)
{
	this->SetLevel(LevelName);
	UGameplayStatics::OpenLevel(this, FName(*this->GameLevels[this->CurrentLevelNumber]), true);
	
}
bool AShooterGame::IsLevelCompleted(FString LevelName)
{
	if (!this->ReadValue.IsEmpty() && this->ReadValue.Contains(LevelName))
	{
		int32 Index = this->ReadValue.Find(LevelName);
		Index = Index + LevelName.Len() + 2;
		while (this->ReadValue[Index] != ' ')
		{
			Index++;
		}
		Index += 1;
		if (this->ReadValue[Index] == 'C')
		{
			return true;
		}
	}
	return false;
}
/*
* SetLevel Checks if nect level exist in GameLevels array, if does, it sets next level as current and return true, otherwise return false
*/
bool AShooterGame::SetLevel(FString LevelToSet)
{
	if (this->GameLevels.Contains(LevelToSet))
	{
		this->CurrentLevelNumber=this->GameLevels.Find(LevelToSet);
		this->CurrentLevelName = LevelToSet;

		return false;
	}
	return true;
}

/*
* Method mostly called in blueprint for settng score and status to certain level (status is hardcoded as 'COMPLETED' for now)
* @param FString - Level which is completed
* @param int32 - score accomplished in that level
*/
void AShooterGame::LevelComplete(FString LevelWhichIsCompleted, float Score, bool Rewrite)
{
	IPlatformFile& Reading = FPlatformFileManager::Get().GetPlatformFile();
	Reading.DeleteFile(*this->GameInfoFile);
	int32 Index = this->ReadValue.Find(LevelWhichIsCompleted);
	Index = Index + LevelWhichIsCompleted.Len() + 1;
	FString sc = FString::SanitizeFloat(Score);

	while ( this->ReadValue[Index] != ' ' )
	{ 
		this->ReadValue.RemoveAt(Index);
	}
	if (!Rewrite)
	{
		this->ReadValue.RemoveAt(Index);
		sc = sc + " COMPLETED";
	}
	
	this->ReadValue.InsertAt(Index, sc);
	FFileHelper::SaveStringToFile(*this->ReadValue, *this->GameInfoFile, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}


/********************************/
/**** GameInfoFile methods ****/

/*
*
* ReadGameInfo - (if GameInfoFile exist) parse GameInfoFile and sets which level need to open while starting the game
* @return false in case game is completed(there are no more levels to read), true otherwise
*
*/
bool AShooterGame::ReadGameInfo()
{
	FFileHelper::LoadFileToString(this->ReadValue,*this->GameInfoFile);

	int32 RetIndex = 0;
	bool CompletedExist;
	FString MakeNew;

	CompletedExist = this->ReadValue.Contains("COMPLETED");
	if (CompletedExist)
	{
		RetIndex = this->ReadValue.Find("COMPLETED", ESearchCase::CaseSensitive, ESearchDir::FromEnd);
		RetIndex += 10;
		if (RetIndex+2 >= this->ReadValue.Len())
		{
			return true;
		}
		while ( this->ReadValue[RetIndex] != ' ')
		{
			MakeNew.AppendChar(this->ReadValue[RetIndex]);
			RetIndex += 1;
		}
		return SetLevel(MakeNew);
	}

	return false;
}

/*
*
* WriteGameInfo - (if GameInfoFile doesn't exist, first time creation of file) create and write levels informations in GameInfoFile
*
*/
void AShooterGame::WriteGameInfo()
{
	IPlatformFile& Reading = FPlatformFileManager::Get().GetPlatformFile();
	Reading.DeleteFile(*this->GameInfoFile);

	FString FileHeader = TEXT("*****************************************************\n");
	FileHeader += TEXT("***************   GAME INFO FILE   ******************\n");
	FileHeader += TEXT("*****************************************************\n");
	FileHeader += TEXT("LEVEL_NAME SCORE STATUS\n\n");
	FString FileContent;
	for (int i = 0; i < this->GameLevels.Num(); i++)
	{	
			FileContent += this->GameLevels[i] + " 0 \n";
	}
	FFileHelper::SaveStringToFile(FileHeader, *this->GameInfoFile, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	FFileHelper::SaveStringToFile(FileContent, *this->GameInfoFile, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

/*
* Reads total score of completed levels
* @return int32 total game score
*/
float AShooterGame::ReadTotalScore()
{
	if ((!FPaths::FileExists(this->GameInfoFile) || this->GameLevels.Num()==0) || ! this->ReadValue.Contains("COMPLETED"))
	{
		return 0;
	}
	float ReturnScore = 0;

	for (int i = 0; i <= this->GameLevels.Num()-1; i++)
	{
		ReturnScore = ReturnScore + this->ReadLevelScore(this->GameLevels[i]);
	}
	return ReturnScore;
}

/*
* Read level score of given level
* @param FString - level which will be searched for
* @return int32 - level score
*/
float AShooterGame::ReadLevelScore(FString LevelName)
{
	if ((! FPaths::FileExists(this->GameInfoFile) || this->GameLevels.Num() == 0) || ! this->ReadValue.Contains("COMPLETED"))
	{
		return 0;
	}
	int32 ReturnScore = 0;
	int32 FindCompleted = 0;
	FString Number;

	int32 Index = this->GameLevels.Find(LevelName);
	FindCompleted = this->ReadValue.Find(this->GameLevels[Index]);
	FindCompleted += this->GameLevels[Index].Len() + 1;

	while (this->ReadValue[FindCompleted] != '\n')
	{
		if (this->ReadValue[FindCompleted] == ' ')
		{
			break;
		}
		Number.AppendChar(this->ReadValue[FindCompleted]);

		FindCompleted += 1;
	}
	ReturnScore += FCString::Atoi(*Number);
	Number.Empty();
	
	return float(ReturnScore);
}

