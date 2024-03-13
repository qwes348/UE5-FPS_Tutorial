// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHUD.h"

void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairTexture)
	{
		// ĵ���� �߾��� ã��
		FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		// �ؽ��� ġ���� ���ݸ�ŭ �������� �����Ͽ� �ؽ��� �߾��� ĵ���� �߾Ӱ� ���ĵǵ��� ��
		FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f),
			Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

		// �߽ɿ� ���ؼ��� �׸�
		FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}
