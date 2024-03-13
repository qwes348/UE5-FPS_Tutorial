// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	if (!CollisionComponent)
	{
		// 스피어를 단순 콜리전 표현으로 사용
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// 스피어의 콜리전 프로파일 이름을 'Projectile'로 설정
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		// 스피어 콜리전 반경을 설정
		CollisionComponent->InitSphereRadius(15.0f);
		// 루트 컴포넌트가 콜리전 컴포넌트가 되도록 설정
		//! 시뮬레이션이 주도할 것이므로 CollisionComponent 를 RootComponent 로 만듭니다.
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		// 이 컴포넌트랄 사용하여 이 발사체의 이동을 주도합니다
		ProjectileMovementComponent = CreateOptionalDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		// 컨텐츠에서 스피어 메시 에셋을 불러옴
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Sphere.Sphere'"));
		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Script/Engine.Material'/Game/SphereMaterial.SphereMaterial'"));
	if (Material.Succeeded())
	{
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	// 3초 후 발사체를 제거합니다.
	InitialLifeSpan = 3.0f;

	// 컴포넌트가 어딘가에 부딪힐 때 호출되는 이벤트
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 발사 방향으로의 발사체 속도를 초기화하는 함수
//! 발사체의 속도가 ProjectileMovementComponent 에 의해 정의되기 때문에 발사 방향만 제공하면 됩니다.
void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// 발사체가 어딘가에 부딪힐 때 호출되는 함수
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
	Destroy();
}

