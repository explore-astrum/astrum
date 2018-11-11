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
	server_selected = true;

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

	MainCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (controller != NULL && controller->IsLocalController()) {
		EnableInput(controller);
		UInputComponent* InputComponent = controller->InputComponent;
		InputComponent->BindAction("SelectObj", IE_Pressed, this, &ASpawnableActor::PickUp).bConsumeInput = false;
	}
}

void ASpawnableActor::AssignToPlayer() {
	MainCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(controller != NULL && controller->IsLocalController()) {
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

	if (assigned && controller != NULL && controller->IsLocalController() && IsOwnedBy(controller)) {

		if (selected) {
			FRotator Rotation = controller->GetControlRotation();
			FVector facing = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
			FVector actor_loc = MainCharacter->GetActorLocation() + FVector(0, 0, 0);
			FVector to_from = actor_loc - GetActorLocation();
			facing.Normalize();
			to_from.Normalize();
			float dot = FVector::DotProduct(facing, to_from);
			SetLocation(actor_loc + facing * 300);
			SetActorLocation(actor_loc + facing * 300);
		}

		if (selected && rotating) {
			FRotator rotation = FRotator(0.0f, 0.0f, 1.0f);
			AddActorLocalRotation(rotation);
		}
	}
	else if (last_seen_time > 0) {
		FVector predicted_location = GetActorLocation() + velocity * DeltaTime;
		if (server_selected)
			SetActorLocation(predicted_location);
		else
			SetActorLocation(last_seen_location);
	}

}

void ASpawnableActor::SetLocation_Implementation(FVector location)
{
	SetLocationMulticast(location);
}

bool ASpawnableActor::SetLocation_Validate(FVector location)
{
	return true;
}

void ASpawnableActor::SetLocationMulticast_Implementation(FVector location)
{
	if (!IsOwnedBy(controller) || !controller->IsLocalController()) {
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

void ASpawnableActor::SetPawnClass_Implementation(UClass* type)
{
	pawnClass = type;
}

bool ASpawnableActor::SetPawnClass_Validate(UClass* type)
{
	return true;
}

void ASpawnableActor::SetIsPawn_Implementation(const bool is_pawn)
{
	isPawn = is_pawn;
}

bool ASpawnableActor::SetIsPawn_Validate(const bool is_pawn)
{
	return true;
}

bool ASpawnableActor::GetIsPawn()
{
	return isPawn;
}

UClass* ASpawnableActor::GetPawn()
{
	return pawnClass;
}

void ASpawnableActor::PlaceObject_Implementation()
{
	server_selected = false;
}

bool ASpawnableActor::PlaceObject_Validate() {
	return true;
}

void ASpawnableActor::SetID_Implementation(const FString &_id) {
	id = _id;
}

bool ASpawnableActor::SetID_Validate(const FString &_id) {
	return true;
}

void ASpawnableActor::RotateX() {
	rotating = !rotating;
}

void ASpawnableActor::SetServerSelected_Implementation(bool _selected) {
	server_selected = _selected;
}

bool ASpawnableActor::SetServerSelected_Validate(bool _selected) {
	return true;
}

FString ASpawnableActor::GetID() {
	return id;
}

void ASpawnableActor::PickUp() {}

bool ASpawnableActor::isSelected() {
	return selected;
}

void ASpawnableActor::SetSelected(bool _selected) {
	selected = _selected;
}

bool ASpawnableActor::GetServerSelected() {
	return server_selected;
}

void ASpawnableActor::OnRep_ChangeMaterial() {
	auto components = GetComponents();
	for (auto component : components)
	{
		if (component->GetFName() == "Object")
		{
			USceneComponent* sc = CastChecked<USceneComponent>(component);
			if (server_selected)
				sc->SetVisibility(false);
			else
				sc->SetVisibility(true);
		}

		if (component->GetFName() == "TempObject")
		{
			USceneComponent* sc = CastChecked<USceneComponent>(component);
			if (server_selected)
				sc->SetVisibility(true);
			else
				sc->SetVisibility(false);
		}
	}
}

void ASpawnableActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpawnableActor, SphereVisualAsset);
	DOREPLIFETIME(ASpawnableActor, server_selected);
	DOREPLIFETIME(ASpawnableActor, id);
	DOREPLIFETIME(ASpawnableActor, pawnClass);
	DOREPLIFETIME(ASpawnableActor, isPawn);
}
