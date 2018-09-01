// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnableActor.h"
#include "UnrealNetwork.h"


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

	selected = true;
	rotating = false;

	//replication
	bReplicates = true;
	//bReplicateMovement = true;

}

// Called when the game starts or when spawned
void ASpawnableActor::BeginPlay()
{
	Super::BeginPlay();
	/*MainCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	EnableInput(controller);

	UInputComponent* InputComponent = controller->InputComponent;

	InputComponent->BindAction("RotateObj", IE_Pressed, this, &ASpawnableActor::RotateX);
	InputComponent->BindAction("RotateObj", IE_Released, this, &ASpawnableActor::RotateX);
	
	InputComponent->BindAction("SelectObj", IE_Pressed, this, &ASpawnableActor::PickUp).bConsumeInput = false;
	*/
}

void ASpawnableActor::AssignToPlayer() {
	MainCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(controller != NULL && controller->IsLocalController()) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "here");
		EnableInput(controller);

		SetOwner(controller);

		UInputComponent* InputComponent = controller->InputComponent;

		InputComponent->BindAction("RotateObj", IE_Pressed, this, &ASpawnableActor::RotateX);
		InputComponent->BindAction("RotateObj", IE_Released, this, &ASpawnableActor::RotateX);

		InputComponent->BindAction("SelectObj", IE_Pressed, this, &ASpawnableActor::PickUp).bConsumeInput = false;
	}
	assigned = true;
}

// Called every frame
void ASpawnableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (assigned && controller != NULL && controller->IsLocalController()) {

		if (selected) {
			FRotator Rotation = controller->GetControlRotation();
			FVector facing = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
			FVector actor_loc = MainCharacter->GetActorLocation() + FVector(0, 0, 0);
			FVector to_from = actor_loc - GetActorLocation();
			facing.Normalize();
			to_from.Normalize();
			float dot = FVector::DotProduct(facing, to_from);
			SetLocation(actor_loc + facing * 300);
			if(IsOwnedBy(controller))
				SetActorLocation(actor_loc + facing * 300);
		}

		if (selected && rotating) {
			FRotator rotation = FRotator(0.0f, 0.0f, 1.0f);
			AddActorLocalRotation(rotation);
		}
	}
	else if (last_seen_time > 0) {
		FVector predicted_location = GetActorLocation() + velocity * DeltaTime;
		SetActorLocation(predicted_location);
	}

}

void ASpawnableActor::SetLocation_Implementation(FVector location)
{
	//SetActorLocation(location);
	SetLocationMulticast(location);
}

bool ASpawnableActor::SetLocation_Validate(FVector location)
{
	return true;
}

void ASpawnableActor::SetLocationMulticast_Implementation(FVector location)
{
	if (!IsOwnedBy(controller)) {
		//SetActorLocation(location);
		float time_now = UGameplayStatics::GetRealTimeSeconds(GetWorld());
		if (last_seen_time > 0) {
			if (time_now - last_seen_time > 0) {
				velocity = ((location - GetActorLocation()) / (time_now - last_seen_time) + velocity) / 2;
			}
		}
		else {
			velocity = FVector(0, 0, 0);
			SetActorLocation(location);
		}
		last_seen_location = location;
		last_seen_time = time_now;
	}
}

void ASpawnableActor::SetMesh(int type)
{

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

void ASpawnableActor::SetMesh_Implementation(const FString &type)
{
	const TCHAR* ctype = *type;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, type);
	SphereVisualAsset = LoadObject<UStaticMesh>(nullptr, ctype);
	//SetActorLocation(location);
	/*SphereVisual->SetStaticMesh(SphereVisualAsset);
	SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SphereVisual->SetWorldScale3D(FVector(1.0f));*/
}

bool ASpawnableActor::SetMesh_Validate(const FString &type)
{
	return true;
}

void ASpawnableActor::OnRep_SetMesh()
{
	SphereVisual->SetStaticMesh(SphereVisualAsset);
	SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SphereVisual->SetWorldScale3D(FVector(1.0f));
}

void ASpawnableActor::SetIntermediateMaterial_Implementation()
{
	SphereVisual->SetMaterial(0, LoadObject<UMaterial>(nullptr, TEXT("/Game/FirstPersonBP/Blueprints/ObjectPlacing.ObjectPlacing")));
}

bool ASpawnableActor::SetIntermediateMaterial_Validate()
{
	return true;
}

void ASpawnableActor::SetMaterial_Implementation(const FString &type)
{
	const TCHAR* ctype = *type;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, type);
	Material = LoadObject<UMaterial>(nullptr, ctype);
	//SphereVisual->SetMaterial(0, Material);
}

bool ASpawnableActor::SetMaterial_Validate(const FString &type)
{
	return true;
}

void ASpawnableActor::OnRep_SetMaterial()
{
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
		SphereVisual->SetMaterial(0, Material);
	}

}

bool ASpawnableActor::isSelected() {
	return selected;
}

void ASpawnableActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpawnableActor, SphereVisualAsset);
	DOREPLIFETIME(ASpawnableActor, Material);
}
