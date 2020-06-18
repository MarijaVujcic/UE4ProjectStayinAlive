#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CrosshairHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ACrosshairHUD : public AHUD
{
	GENERATED_BODY()
	ACrosshairHUD(const FObjectInitializer& ObjectInitializer);
	virtual void DrawHUD();
private:
	UTexture2D* CrosshairTex;
};
