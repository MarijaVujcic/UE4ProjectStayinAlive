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
	this->GameInfoFile = "GameInfo.txt";
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
	if (this->GameLevels.Num() == 0)
	{
		// pozvati neki error widget ILI TO U BLUEPRINTU
	}
	if (FPaths::FileExists(FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + this->GameInfoFile))
	{
		this->GameCompleted = this->ReadGameInfo();
	}
	else
	{
		this->WriteGameInfo("");
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

		//HUDClass = ACrosshairHUD::StaticClass();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Uslo u begin open level %d"), this->CurrentLevelNumber));
		UGameplayStatics::OpenLevel(this, FName(*this->GameLevels[this->CurrentLevelNumber]), true);
	}
}
void AShooterGame::RepeatLevel(FString LevelName)
{
	this->SetLevel(LevelName);
	UGameplayStatics::OpenLevel(this, FName(*this->GameLevels[this->CurrentLevelNumber]), true);
	
}
/*
* SetLevel Checks if nect level exist in GameLevels array, if does, it sets next level as current and return true, otherwise return false
*/
bool AShooterGame::SetLevel(FString LevelToSet)
{
	// mozda mi treba neki uvjet jos amo
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
void AShooterGame::LevelComplete(FString LevelWhichIsCompleted, float Score)
{
	WriteGameInfo(LevelWhichIsCompleted, Score);
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
	FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + this->GameInfoFile;
	FFileHelper::LoadFileToString(this->ReadValue,*(FilePath));

	int32 RetIndex = 0;
	bool CompletedExist;
	FString MakeNew;

	CompletedExist = this->ReadValue.Contains("COMPLETED");
	if (CompletedExist)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Uslo u completed exist")));

		RetIndex = this->ReadValue.Find("COMPLETED", ESearchCase::CaseSensitive, ESearchDir::FromEnd);
		RetIndex += 10;
		if (RetIndex+2 >= this->ReadValue.Len())
		{
			return true;
		}
		while ( this->ReadValue[RetIndex] != ' ')
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Uslo u while")));

			MakeNew.AppendChar(this->ReadValue[RetIndex]);
			RetIndex += 1;
		}
		return SetLevel(MakeNew);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("waat")));

	return false;
}

/*
*
* WriteGameInfo - (if GameInfoFile doesn't exist, first time creation of file) create and write levels informations in GameInfoFile
*
*/
void AShooterGame::WriteGameInfo(FString LevelWhichIsCompleted, float Score)
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

/*
* Read level score of given level
* @param FString - level which will be searched for
* @return int32 - level score
*/
int32 AShooterGame::ReadLevelScore(FString LevelName)
{
	if (!FPaths::FileExists(FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + this->GameInfoFile) || this->GameLevels.Num() == 0 /*|| !this->ReadValue.Contains("COMPLETED")*/)
	{
		return -1;
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
	
	return ReturnScore;
}

/*
* Rewrite game info file with new informations of certain level
* @param int32 - level score to rewrite
* @param FString - level name to rewrite score for
*/
void AShooterGame::RewriteScore(int32 LevelScore, FString LevelName)
{
	IPlatformFile& Reading = FPlatformFileManager::Get().GetPlatformFile();
	FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSavedDir()) + this->GameInfoFile;
	const uint8 NewScore = LevelScore;
	const uint8 Complete = *"COMPLETED\n";

	IFileHandle* HandleWriting = Reading.OpenWrite(*FilePath);
	HandleWriting->Seek(this->ReadValue.Find(LevelName) + LevelName.Len() + 2);
	HandleWriting->Write(&NewScore,  sizeof(NewScore));
	HandleWriting->Write(&Complete, sizeof(Complete));
	delete HandleWriting;
}
