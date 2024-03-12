// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "FPSCharacter.generated.h"

UCLASS()
class FPS_TUTORIAL_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 앞으로 이동 및 뒤로 이동 입력을 처리합니다.
	//! UFUNCTION 매크로는 엔진이 함수를 인식하여 시리얼라이제이션 및 다른 엔진 기능에 포함하도록 합니다.
	UFUNCTION()
	void MoveForward(float value);

	// 오른쪽 이동 및 왼쪽 이동 입력을 처리합니다.
	UFUNCTION()
	void MoveRight(float value);

	// 키가 눌릴 경우 점프 플래그를 설정합니다.
	UFUNCTION()
	void StartJump();

	// 키가 떼어질 경우 점프 플래그를 지웁니다.
	UFUNCTION()
	void StopJump();

	// FPS 카메라입니다.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	// 일인칭 메시(팔)로, 소유 플레이어에게만 보입니다.
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FPSMesh;
};
