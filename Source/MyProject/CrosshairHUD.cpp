// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairHUD.h"
#include "Engine/Texture2D.h" 
#include "UObject/ConstructorHelpers.h"
#include "Engine/Canvas.h"

ACrosshairHUD::ACrosshairHUD(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
    // Set the crosshair texture
    static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/Meshes/pic/crosshair.crosshair'"));
    CrosshairTex = CrosshairTexObj.Object;
}

void ACrosshairHUD::DrawHUD()
{
    Super::DrawHUD();
    const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
    // offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
    const FVector2D CrosshairDrawPosition((Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5)),
        (Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)));
    // draw the crosshair
    FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
    TileItem.BlendMode = SE_BLEND_Translucent;
    Canvas->DrawItem(TileItem);
}