// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnableActor.h"
#include "UnrealNetwork.h"
#include "DrawDebugHelpers.h"


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

	OnActorBeginOverlap.AddDynamic(this, &ASpawnableActor::OnOverlap);

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

			//check if in owned land
			TArray<FLand> owned_lands = Cast<AAstrumPlayerController>(controller)->GetProperties();
			bool placeable = false;
			for (int i = 0; i < owned_lands.Num(); i++) {
				FLand owned_land = owned_lands[i];
				FVector current_loc = GetActorLocation();
				if (current_loc.X > owned_land.min.X
					&& current_loc.X < owned_land.max.X
					&& current_loc.Y > owned_land.min.Y
					&& current_loc.Y < owned_land.max.Y) {
					placeable = true;
					break;
				}
				else if(i >= owned_lands.Num() - 1){
					placeable = false;
				}
			}

			if (placeable && !can_place)
				MakePlaceable(true);
			if (!placeable && can_place)
				MakePlaceable(false);
		}

		if (selected && rotating) {
			FRotator rotation = FRotator(0.0f, 0.0f, 1.0f);
			AddActorLocalRotation(rotation);
		}
	}
	else if (last_seen_time > 0) {
		FVector predicted_location = GetActorLocation() + velocity * DeltaTime;
		if (server_selected) {
			SetActorLocation(predicted_location);
			if (HasAuthority()) {
				ProcessedLocationChange.Broadcast(id, predicted_location);
			}
		}
		else {
			SetActorLocation(last_seen_location);
			if (HasAuthority()) {
				ProcessedLocationChange.Broadcast(id, last_seen_location);
			}
		}
	}

}

void ASpawnableActor::MakePlaceable(bool place) {
	can_place = place;

	auto components = GetComponents();
	for (auto component : components)
	{
		if (component->GetFName() == "TempObject")
		{
			UStaticMeshComponent* sc = Cast<UStaticMeshComponent>(component);
			
			for (int i = 0; i < sc->GetNumMaterials(); i++)
			{
				UMaterialInterface * m = sc->GetMaterial(i);
				UMaterialInstanceDynamic* matInstance = sc->CreateDynamicMaterialInstance(i, m);

				if (matInstance != nullptr)
					matInstance->SetVectorParameterValue("TColor", can_place ? FVector(0.0, 0.0, 1.0) : FVector(1.0, 0.0, 0.0));
			}
		}
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
	//if (!IsOwnedBy(controller) || !controller->IsLocalController()) {
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
	//}
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

void ASpawnableActor::PlaceObject_Implementation(bool _placed)
{
	server_selected = false;
	placed = _placed;
}

bool ASpawnableActor::PlaceObject_Validate(bool _placed) {
	return true;
}

bool ASpawnableActor::GetIsPlaced() {
	return placed;
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

FString ASpawnableActor::GetUserID() {
	return userid;
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

void ASpawnableActor::OnRep_ChangeCombinations() {
	for (int i = 0; i < combinedRelics.Num(); i++) { // just use all current combos for now
		FRelicState combinedRelic = combinedRelics[i];
		if (combinedRelic.state == ERelicProcess::PRE) {
			EAction action = possibleCombinationTypes.FindRef(combinedRelic.relic->category);
			switch (action) {
				case EAction::TURN_ON:
					TurnMeOn();
					break;
				case EAction::PAINT:
					PaintMe(combinedRelic.relic);
					break;
				case EAction::EVOLVE:
					EvolveMe(combinedRelic.relic);
					break;
				case EAction::CUSTOM:
					CustomComboAction(combinedRelic.relic);
				default:
					break;
			}
		}

	}
}

void ASpawnableActor::TurnMeOn() {
	turnedOn = true;
}

void ASpawnableActor::PaintMe(ASpawnableActor* actor) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("painting"));
}

void ASpawnableActor::EvolveMe(ASpawnableActor* actor) {
	//to do
	actor->StartEvolution();
}

void ASpawnableActor::StartEvolution() {
	//to do... kick off animation?
}

void ASpawnableActor::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
}

void ASpawnableActor::AddCombinationRelic_Implementation(FRelicState relicState)
{
	combinedRelics.Add(relicState);
}

bool ASpawnableActor::AddCombinationRelic_Validate(FRelicState relicState)
{
	return true;
}

FRelic ASpawnableActor::CreateRelicFromProperties()
{
	FRelic r;
	r.id = id;
	r.isPawn = isPawn;
	r.icon = icon;
	r.pawnClass = pawnClass;
	r.blueprint = blueprint;
	r.category = category;
	return r;
}

void ASpawnableActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASpawnableActor, SphereVisualAsset);
	DOREPLIFETIME(ASpawnableActor, server_selected);
	DOREPLIFETIME(ASpawnableActor, id);
	DOREPLIFETIME(ASpawnableActor, pawnClass);
	DOREPLIFETIME(ASpawnableActor, isPawn);
	DOREPLIFETIME(ASpawnableActor, combinedRelics);
	DOREPLIFETIME(ASpawnableActor, placed);
}
