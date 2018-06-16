// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterMovable.h"

// Sets default values
ACharacterMovable::ACharacterMovable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = sphere;
	sphere->InitSphereRadius(40.0f);
	sphere->SetCollisionProfileName(TEXT("Pawn"));

	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}

	selected = false;
	rotating = false;
}

// Called when the game starts or when spawned
void ACharacterMovable::BeginPlay()
{
	Super::BeginPlay();
	MainCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	EnableInput(controller);

	controller->InputComponent->BindAction("RotateObj", IE_Pressed, this, &ACharacterMovable::RotateX);
	controller->InputComponent->BindAction("RotateObj", IE_Released, this, &ACharacterMovable::RotateX);

}

// Called every frame
void ACharacterMovable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	else {
		selected = false;
	}

	if (selected && rotating) {
		FRotator rotation = FRotator(0.0f, 0.0f, 1.0f);
		AddActorLocalRotation(rotation);
	}

}

void ACharacterMovable::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	SphereVisual->SetStaticMesh(SphereVisualAsset);
	SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SphereVisual->SetWorldScale3D(FVector(0.8f));

	Super::PostEditChangeProperty(e);
}

void ACharacterMovable::RotateX() {
	rotating = !rotating;
}