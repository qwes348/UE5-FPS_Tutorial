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
		// ���Ǿ �ܼ� �ݸ��� ǥ������ ���
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// ���Ǿ��� �ݸ��� �������� �̸��� 'Projectile'�� ����
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		// ���Ǿ� �ݸ��� �ݰ��� ����
		CollisionComponent->InitSphereRadius(15.0f);
		// ��Ʈ ������Ʈ�� �ݸ��� ������Ʈ�� �ǵ��� ����
		//! �ùķ��̼��� �ֵ��� ���̹Ƿ� CollisionComponent �� RootComponent �� ����ϴ�.
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		// �� ������Ʈ�� ����Ͽ� �� �߻�ü�� �̵��� �ֵ��մϴ�
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
		// ���������� ���Ǿ� �޽� ������ �ҷ���
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

	// 3�� �� �߻�ü�� �����մϴ�.
	InitialLifeSpan = 3.0f;

	// ������Ʈ�� ��򰡿� �ε��� �� ȣ��Ǵ� �̺�Ʈ
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

// �߻� ���������� �߻�ü �ӵ��� �ʱ�ȭ�ϴ� �Լ�
//! �߻�ü�� �ӵ��� ProjectileMovementComponent �� ���� ���ǵǱ� ������ �߻� ���⸸ �����ϸ� �˴ϴ�.
void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// �߻�ü�� ��򰡿� �ε��� �� ȣ��Ǵ� �Լ�
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
	Destroy();
}

