// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnableActor.h"


// Sets default values
ASpawnableActor::ASpawnableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = sphere;
	sphere->InitSphereRadius(40.0f);
	sphere->SetCollisionProfileName(TEXT("Pawn"));

	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);

	selected = false;
	rotating = false;

}

// Called when the game starts or when spawned
void ASpawnableActor::BeginPlay()
{
	Super::BeginPlay();
	MainCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	EnableInput(controller);

	controller->InputComponent->BindAction("RotateObj", IE_Pressed, this, &ASpawnableActor::RotateX);
	controller->InputComponent->BindAction("RotateObj", IE_Released, this, &ASpawnableActor::RotateX);
	
	controller->InputComponent->BindAction("SelectObj", IE_Pressed, this, &ASpawnableActor::PickUp);
	
}

// Called every frame
void ASpawnableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (selected) {
		FRotator Rotation = controller->GetControlRotation();
		FVector facing = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		FVector actor_loc = MainCharacter->GetActorLocation() + FVector(0, 0, 0);
		FVector to_from = actor_loc - GetActorLocation();
		facing.Normalize();
		to_from.Normalize();
		float dot = FVector::DotProduct(facing, to_from);
		SetActorLocation(actor_loc + facing * 300);
	}

	if (selected && rotating) {
		FRotator rotation = FRotator(0.0f, 0.0f, 1.0f);
		AddActorLocalRotation(rotation);
	}

}

void ASpawnableActor::SetMesh(int type)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat(type));

	if (type == 1) {
		SphereVisualAsset = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	}
	else if (type == 2) {
		SphereVisualAsset = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus"));
	}
	SphereVisual->SetStaticMesh(SphereVisualAsset);
	SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SphereVisual->SetWorldScale3D(FVector(1.0f));
}

void ASpawnableActor::SetMesh(FString type)
{
	const TCHAR* ctype = *type;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, type);
	SphereVisualAsset = LoadObject<UStaticMesh>(nullptr, ctype);
	SphereVisual->SetStaticMesh(SphereVisualAsset);
	SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SphereVisual->SetWorldScale3D(FVector(1.0f));
}

void ASpawnableActor::SetMaterial(FString type)
{
	const TCHAR* ctype = *type;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, type);
	Material = LoadObject<UMaterial>(nullptr, ctype);
	SphereVisual->SetMaterial(0, Material);
}

void ASpawnableActor::RotateX() {
	rotating = !rotating;
}

void ASpawnableActor::PickUp() {
	if (!selected) {
		FRotator Rotation = controller->GetControlRotation();
		FVector facing = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		FVector actor_loc = MainCharacter->GetActorLocation() + FVector(0, 0, 0);
		FVector to_from = actor_loc - GetActorLocation();
		facing.Normalize();
		to_from.Normalize();
		float dot = FVector::DotProduct(facing, to_from);


		if (dot < -0.99) {
			SetActorLocation(actor_loc + facing * 300);
			selected = true;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat(dot));
		}
	}
	else {
		selected = false;
	}

}