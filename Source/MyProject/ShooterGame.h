#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "ShooterGame.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AShooterGame : public AGameModeBase
{
	GENERATED_BODY()
public:

	AShooterGame();
	/*UFUNCTION(BlueprintCallable)
	void EndGame();*/
	UFUNCTION(BlueprintCallable)
	void LevelComplete(FString LevelWhichIsCompleted, int32 Score);
	UFUNCTION(BlueprintCallable)
	bool LoadNextLevel();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
		class UUserWidget* LevelCompleteWidget;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
		TSubclassOf<class UUserWidget> DeafultGameCompleteWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
		class UUserWidget* GameCompleteWidget;

	FTimerHandle LevelSwap;
	FString GameInfoFile;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Files")
		FString EndGameMap;
private:
	void BeginPlay() override;

	// names of levels to be loaded in game
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Levels")
		TArray<FString> GameLevels;

	class APlayerController* DefaultPlayerController;
	int32 CurrentLevelNumber;
	FString NextLevelName;
	FString CurrentLevelName;
	FString ReadValue;

	void CheckLevel();
	bool ReadGameInfo();
	void WriteGameInfo(FString LevelWhichIsCompleted, int32 Score=0);

};
