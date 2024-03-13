// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "FPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class FPS_TUTORIAL_API AFPSHUD : public AHUD
{
	GENERATED_BODY()
	
protected:

	// 화면 중앙에 그려짐
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairTexture;

public:

	// HUD의 기본 드로콜
	virtual void DrawHUD() override;
};
