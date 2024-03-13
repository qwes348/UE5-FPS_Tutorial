// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FPSProjectile.h"
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

	// 스폰할 발사체 클래스입니다.
	//! EditDefaultsOnly 는 발사체 클래스를 블루프린트의 각 인스턴스가 아닌 블루프린트에서만 디폴트로 설정할 수 있다는 의미입니다.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFPSProjectile> ProjectileClass;

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

	// 발사체를 발사를 처리하는 함수입니다.
	UFUNCTION()
	void Fire();

	// 카메라 위치로부터의 총구 오프셋입니다.
	/*EditAnywhere 를 사용하면 블루프린트 에디터의 디폴트 모드 내에서나 캐릭터의 아무 인스턴스의 디테일 탭 내에서 총구 오프셋 값을 변경할 수 있습니다.
	BlueprintReadWrite 지정자를 사용하면 블루프린트 내에서 총구 오프셋의 값을 구하고 설정할 수 있습니다.*/
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = GamePlay)
	FVector MuzzleOffset;
};
