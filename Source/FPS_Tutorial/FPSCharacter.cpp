// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 일인칭 카메라 컴포넌트를 생성합니다.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	//  캡슐 컴포넌트에 카메라 컴포넌트를 어태치합니다
	//! 이 코드는 UCameraComponent 를 생성한 후 캐릭터의 CapsuleComponent 에 어태치합니다.
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// 카메라가 눈 약간 위에 위치하도록 합니다.
	/*SetRelativeLocation 가 컴포넌트의 디폴트를 설정하지만, 이전 값은 계속 에디터에서 설정됩니다.
	이 문제를 해결하려면 블루프린트 에디터 를 열고 FPSCameraComponent 를 클릭한 후 디테일 패널에서 트랜스폼(Transform) > 위치(Location) 값을 찾습니다.
	해당 값 옆에 있는 디폴트로 리셋(Reset to Default) 아이콘을 클릭합니다.*/
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	// 폰이 카메라 회전을 제어하도록 합니다.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// 소유 플레이어의 일인칭 메시 컴포넌트를 생성합니다.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);

	// 소유 플레이어만 이 메시를 볼 수 있습니다.
	/*SetOnlyOwnerSee 는 이 메시가 이 캐릭터에 빙의한 PlayerController 에만 보인다는 것을 의미합니다.
	이 코드는 메시를 카메라에 어태치하고 일부 환경 그림자를 비활성화하기도 합니다.
	팔이 그림자를 드리우도록 하면 일인칭 캐릭터가 단일 메시 같다는 느낌이 사라집니다.*/
	FPSMesh->SetOnlyOwnerSee(true);

	//FPS 메시를 FPS 카메라에 어태치합니다.
	FPSMesh->SetupAttachment(FPSCameraComponent);

	// 일부 환경 그림자를 비활성화하여 단일 메시 같은 느낌을 보존합니다.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// 소유 플레이어의 몸(바디)메시를 숨김
	GetMesh()->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(GEngine != nullptr);

	// 디버그 메시지를 5초간 표시합니다. 
	// -1 '키' 값 실행인자가 메시지가 업데이트되거나 새로고침되지 않도록 방지합니다.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 'movement' 바인딩 구성
	//! InputComponent 는 입력 데이터 처리 방식을 정의하는 컴포넌트입니다. InputComponent 는 입력을 수신할 액터에 어태치할 수 있습니다.
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	// 'look' 바인딩 구성
	//! Character 베이스 클래스는 다음과 같은 이름의 필수 함수 2개를 정의합니다.
	//! AddControllerYawInput, AddControllerPtchInput
	/*감도 또는 축 반전 지원을 추가하는 등 그 밖의 처리를 수행하고자 한다면, 입력 값이 함수에 전달되기 전에 조정하는 자체 함수를 추가하면 됩니다.
	하지만 여기서는 입력을 바로 AddControllerYawInput 및 AddControllerPitchInput 함수에 바인딩하도록 하겠습니다.*/
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	// 'action' 바인딩 구성
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
}

void AFPSCharacter::MoveForward(float value)
{
	// 어디가 '앞'인지 찾고, 플레이어가 해당 방향으로 이동하고자 한다는 것을 기록합니다.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value);
}

void AFPSCharacter::MoveRight(float value)
{
	// 어디가 '오른쪽'인지 찾고, 플레이어가 해당 방향으로 이동하고자 한다는 것을 기록합니다.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value);
}

void AFPSCharacter::StartJump()
{
	// ACharcter클래스에 구현돼있는 점프의 사용되는 변수
	bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::Fire()
{
	// 발사체 발사를 시도합니다
	if (ProjectileClass)
	{
		// 카메라 트랜스폼을 구합니다
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// MuzzleOffset이 카메라 살짝 앞에서 발사체를 스폰하도록 설정합니다.
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// MuzzleOffset을 카메라 스페이스에서 월드 스페이스로 변환합니다.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// 조준이 살짝 위를 향하도록 왜곡합니다.
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// 총구에 발사체를 스폰합니다.
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// 발사체의 초기 탄도를 설정합니다.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

