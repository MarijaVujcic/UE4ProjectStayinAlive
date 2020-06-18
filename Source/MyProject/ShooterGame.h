#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "ShooterGame.generated.h"

/**
 * Game Mode class with main logic of game
 */
UCLASS()
class MYPROJECT_API AShooterGame : public AGameModeBase
{
	GENERATED_BODY()

public:

	AShooterGame();
	UFUNCTION(BlueprintCallable)
	void LevelComplete(FString LevelWhichIsCompleted, float Score);

	UFUNCTION(BlueprintCallable)
	bool SetLevel(FString LevelToSet);

	UFUNCTION(BlueprintCallable)
	int32 ReadTotalScore();

	UFUNCTION(BlueprintCallable)
		int32 ReadLevelScore(FString LevelName);

	UFUNCTION(BlueprintCallable)
		void RewriteScore(int32 LevelScore, FString LevelName);

	UFUNCTION(BlueprintCallable)
	bool ReadGameInfo();

	UFUNCTION(BlueprintCallable)
		void StartLevel();

	UFUNCTION(BlueprintCallable)
		void RepeatLevel(FString LevelName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
		class UUserWidget* LevelCompleteWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
		class UUserWidget* GameCompleteWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
		FString EndGameMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Files")
		FString GameInfoFile;
	UPROPERTY( BlueprintReadOnly)
		FString NextLevelName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool GameCompleted;

private:

	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Levels")
		TArray<FString> GameLevels;

	class APlayerController* DefaultPlayerController;
	int32 CurrentLevelNumber;
	
	FString CurrentLevelName;
	FString ReadValue;


	void WriteGameInfo(FString LevelWhichIsCompleted, float Score=0.0f);

};
