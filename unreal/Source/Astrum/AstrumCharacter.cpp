// Fill out your copyright notice in the Description page of Project Settings.

#include "AstrumCharacter.h"
#include "SpawningWidget.h"
#include "UnrealNetwork.h"


// Sets default values
AAstrumCharacter::AAstrumCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> hudWidgetClass(TEXT("/Game/FirstPersonBP/Blueprints/HUD.HUD_C"));
	static ConstructorHelpers::FClassFinder<UUserWidget> mainMenuWidgetClass(TEXT("/Game/FirstPersonBP/Blueprints/MainMenu.MainMenu_C"));
	//UWidgetComponent hudc;
	//auto mmc = UWidgetComponent();
	//hudc.SetWidgetClass(hudWidgetClass.Class);
	//mmc.SetWidgetClass(mainMenuWidgetClass.Class);

	HUDWidgetClass = hudWidgetClass.Class;
	MainMenuWidgetClass = mainMenuWidgetClass.Class;
	//MainMenu = mmc.GetUserWidgetObject();

	bReplicateMovement = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AAstrumCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAstrumCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!init) {
		owner = Cast<AAstrumPlayerController>(GetController());
		SetupInventory();
		MovePlayerToStartingPosition();
	}
}

// Called to bind functionality to input
void AAstrumCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AAstrumCharacter::ToggleInventory);
	PlayerInputComponent->BindAction("SelectLeft", IE_Pressed, this, &AAstrumCharacter::ScrollLeft);
	PlayerInputComponent->BindAction("SelectRight", IE_Pressed, this, &AAstrumCharacter::ScrollRight);

}

void AAstrumCharacter::SetupInventory()
{
	if (owner->IsLocalController()) {
		init = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		CreateMyWidget();
		owner->SetProperties();
	}
}

void AAstrumCharacter::CreateMyWidget()
{
	FString levelname = GetWorld()->GetMapName();
	levelname.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (levelname == "Astrum" || levelname == "Tester") {
		CurrentWidget = CreateWidget<UUserWidget>(owner, HUDWidgetClass);
		owner->bShowMouseCursor = false;
		EnableInput(owner);

		auto SpawnableWidget = Cast<USpawningWidget>(CurrentWidget);
		SpawnableWidget->GetNewOptions();
	}
	else {
		CurrentWidget = CreateWidget<UUserWidget>(owner, MainMenuWidgetClass);
		owner->bShowMouseCursor = true;
		DisableInput(owner);
	}
	CurrentWidget->AddToViewport();

	//set up all the inventory in the widget
}

void AAstrumCharacter::MovePlayerToStartingPosition()
{
	if (GetWorld()->GetMapName() == "Astrum") {
		FLand firstOwnedLand = owner->GetProperties()[0];
		FVector spawn = FVector((firstOwnedLand.min.X + firstOwnedLand.max.X) / 2.0, (firstOwnedLand.min.Y + firstOwnedLand.max.Y) / 2.0, 300);
		SetCharacterLocation(spawn);
	}
}

void AAstrumCharacter::ToggleInventory()
{
	auto sw = Cast<USpawningWidget>(CurrentWidget);
	sw->ToggleInventory();
	inventoryOn = !inventoryOn;
}

void AAstrumCharacter::ScrollLeft()
{
	auto sw = Cast<USpawningWidget>(CurrentWidget);
	sw->ScrollLeft();
}

void AAstrumCharacter::ScrollRight()
{
	auto sw = Cast<USpawningWidget>(CurrentWidget);
	sw->ScrollRight();
}

void AAstrumCharacter::SetCharacterLocation_Implementation(FVector location)
{
	SetActorLocation(location);
}

bool AAstrumCharacter::SetCharacterLocation_Validate(FVector location)
{
	return true;
}

void AAstrumCharacter::CheckForObjectHit()
{
	TArray<FLand> ownedLand = owner->GetProperties();
	FVector currentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	for (int i = 0; i < ownedLand.Num(); i++) {
		FLand land = ownedLand[i];
		if (currentLocation.X < land.max.X
			&& currentLocation.X > land.min.X
			&& currentLocation.Y < land.max.Y
			&& currentLocation.Y > land.min.Y) {

			FCollisionQueryParams TraceParams(FName(TEXT("Trace Params")), true, this); //ignore self
			TraceParams.bTraceComplex = true;

			FHitResult HitOut = FHitResult(ForceInit);

			GetWorld()->LineTraceSingleByChannel(
				HitOut,		//result
				currentLocation,	//start
				currentLocation + forwardVector * pickupDistance, //end
				ECC_WorldDynamic, //collision channel
				TraceParams
			);

			//Hit any Actor?
			if (HitOut.GetActor() != NULL) {
				auto actor = Cast<ASpawnableActor>(HitOut.GetActor());
				if (actor) { // check if spawnable actor
					if (actor->GetServerSelected()) {
						PutActorDown(actor);
					}
					else {
						if (!inventoryOn) {
							ServerChangeOwner(actor); //give it to me
						}
					}
				}

			}
			else {
				if (lastSpawned && lastSpawned->GetServerSelected()) {
					PutActorDown(lastSpawned);
				}
			}
		}
	}
}

void AAstrumCharacter::PutActorDown(ASpawnableActor* actor)
{
	actor->SetSelected(false);
	if (actor->GetIsPawn()) {
		ReplacePawn(actor); // replace with real pawn
	}
	else {
		actor->PlaceObject(); // put it down
		RemoveMyselfAsOwner(actor);
		PlaceObjectLocal();
	}
}

void AAstrumCharacter::ReplacePawn_Implementation(ASpawnableActor* actor)
{
	UClass* pawnClass = actor->GetPawn();
	GetWorld()->SpawnActor<AActor>(pawnClass, GetActorLocation() + FVector(0, 0, 100.0), FRotator::ZeroRotator);
	actor->Destroy();
	PlaceObjectLocal();
}

bool AAstrumCharacter::ReplacePawn_Validate(ASpawnableActor* actor)
{
	return true;
}

void AAstrumCharacter::ServerChangeOwner_Implementation(ASpawnableActor* actor)
{
	if (actor->GetID() == owner->GetUserID()) {
		actor->SetOwner(owner);
		AssignToLocalController(actor);
	}
}

bool AAstrumCharacter::ServerChangeOwner_Validate(ASpawnableActor* actor)
{
	return true;
}

void AAstrumCharacter::RemoveMyselfAsOwner_Implementation(ASpawnableActor* actor)
{
	actor->SetOwner(NULL);
}

bool AAstrumCharacter::RemoveMyselfAsOwner_Validate(ASpawnableActor* actor)
{
	return true;
}

void AAstrumCharacter::PlaceObjectLocal_Implementation()
{
	auto sw = Cast<USpawningWidget>(CurrentWidget);
	sw->PlaceObject();
}

void AAstrumCharacter::AssignToLocalController_Implementation(ASpawnableActor* actor)
{
	actor->AssignToPlayer();
	actor->SetSelected(true);
	actor->SetServerSelected(true);
	lastSpawned = actor;
}

void AAstrumCharacter::MoveRelicToMe_Implementation(UClass* actorType, const FString &relicID)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), actorType, FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++) {
		ASpawnableActor* actor = Cast<ASpawnableActor>(FoundActors[i]);
		if (actor) {
			if (actor->GetID() == relicID) {
				ServerChangeOwner(actor);
				lastSpawned = actor;
			}
		}
	}
}

bool AAstrumCharacter::MoveRelicToMe_Validate(UClass* actorType, const FString &relicID)
{
	return true;
}

void AAstrumCharacter::MoveRelicToDefault()
{
	lastSpawned->SetSelected(false);
	lastSpawned->PlaceObject();
	lastSpawned->SetLocation(defaultLocation);
	SetActorLocation(defaultLocation);
	RemoveMyselfAsOwner(lastSpawned);
}

void AAstrumCharacter::GoToServer()
{
	owner->GoToServer();
}

void AAstrumCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAstrumCharacter, lastSpawned);
}