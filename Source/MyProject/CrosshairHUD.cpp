#include "CrosshairHUD.h"
#include "Engine/Texture2D.h" 
#include "UObject/ConstructorHelpers.h"
#include "Engine/Canvas.h"

ACrosshairHUD::ACrosshairHUD(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
    static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/Meshes/pic/crosshair.crosshair'"));
    CrosshairTex = CrosshairTexObj.Object;
}

void ACrosshairHUD::DrawHUD()
{
    Super::DrawHUD();
    const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
    const FVector2D CrosshairDrawPosition((Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5)),
        (Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)));
    FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
    TileItem.BlendMode = SE_BLEND_Translucent;
    Canvas->DrawItem(TileItem);
}