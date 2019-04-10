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

	//add sounds
	static ConstructorHelpers::FObjectFinder<USoundCue> astrum2CueObj(TEXT("'/Game/FirstPersonBP/Sounds/Astrum2.Astrum2'"));
	USoundCue* astrum2Cue = astrum2CueObj.Object;
	UAudioComponent* astrum2AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	astrum2AudioComponent->bAutoActivate = false;
	astrum2AudioComponent->SetupAttachment(RootComponent);
	astrum2AudioComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	songs.Add(astrum2AudioComponent);
	soundCues.Add(astrum2Cue);

	bReplicateMovement = true;
	bReplicates = true;

	//tcp = new ATCPConnection();

}

void AAstrumCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	for (int i = 0; i < songs.Num(); i++) {
		if (songs.Num() > i && soundCues.Num() > i && songs[i]->IsValidLowLevelFast()) {
			songs[i]->SetSound(soundCues[i]);
		}
	}
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
	
	//play sounds if far away
	if (init) {
		CheckMusic();
		if(inventoryOn)
			CheckForUpgrades();

		//SetCharacterLocation(FVector(0, 0, 500));
		//FScreenshotRequest::RequestScreenshot(false);
		/*if (move_on && 6300 * ind < 403200) {
			FVector camloc = FVector(403200 * 2 - 200, 403200 * 2 - 200, 30000) - FVector(indx * 6300 + 6300, 6300 * ind, 0);

			FCollisionQueryParams TraceParams(FName(TEXT("Trace Params")), true, this); //ignore self
			TraceParams.bTraceComplex = true;

			FHitResult HitOut = FHitResult(ForceInit);

			GetWorld()->LineTraceSingleByChannel(
				HitOut,		//result
				camloc,	//start
				camloc + FVector(0, 0, -1) * 1000000, //end
				ECC_Visibility, //collision channel
				TraceParams
			);

			//Hit any Actor?
			if (HitOut.GetActor() != NULL) {
				camloc.Z = HitOut.ImpactPoint.Z + 100;
				//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Yellow, camloc.ToString());
				SetActorLocation(camloc);
			}
			last_timey = numticks;
			move_on = false;
		}

		if (numticks > last_timey + 1 * 60 && !move_on) {

			/*FRotator NewRotation = FRotator(0, 0, 90);
			FQuat QuatRotation = FQuat(NewRotation);
			AddActorLocalRotation(QuatRotation);
			numrots += 1;*/

		/*	FScreenshotRequest::RequestScreenshot(FString("x") + FString::FromInt(indx) + FString("y") + FString::FromInt(ind) + FString("rightforward"), false, true);

			move_on = true;
			numrots = 0;
		}

		numticks++;

		if (ind * 6300 >= 403200) {
			ind = 0;
		}*/
	}
}

void AAstrumCharacter::CheckMusic()
{
	TArray<FLand> ownedLand = owner->GetProperties();
	FVector2D currentLocation = FVector2D(GetActorLocation().X, GetActorLocation().Y);
	bool farFromHome = false;
	for (int i = 0; i < ownedLand.Num(); i++) {
		FLand land = ownedLand[i];
		FVector2D midpoint = FVector2D((land.max.X + land.min.X) / 2.0, (land.min.X + land.min.Y) / 2.0);
		float distFromHome = midpoint.Distance(midpoint, currentLocation);

		if (distFromHome > 25000) {
			farFromHome = true;
			break;
		}
	}

	if (farFromHome && songs.Num() > 0 && !songs[0]->IsPlaying())
		songs[0]->Play();
}

// Called to bind functionality to input
void AAstrumCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AAstrumCharacter::ToggleInventory);
	PlayerInputComponent->BindAction("SelectLeft", IE_Pressed, this, &AAstrumCharacter::ScrollLeft);
	PlayerInputComponent->BindAction("SelectRight", IE_Pressed, this, &AAstrumCharacter::ScrollRight);
	PlayerInputComponent->BindAction("SelectObj", IE_Pressed, this, &AAstrumCharacter::CheckForObjectHit);
	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &AAstrumCharacter::PutBackInInventory);

}

void AAstrumCharacter::SetupInventory()
{
	if (owner && owner->IsLocalController()) {
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

	if (!inventoryOn && lastSpawned && lastSpawned->server_selected) {
		MoveRelicToDefault();
	}
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
	if (ApplyUpgrades()) {
		//destroy lastspawned object in some way
		return;
	}

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
	if (actor->GetUserID() == owner->GetUserID()) {
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
	//lastSpawned = actor;
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

void AAstrumCharacter::MoveRelicToDefault_Implementation()
{
	if (lastSpawned) {
		lastSpawned->SetSelected(false);
		lastSpawned->PlaceObject();
		lastSpawned->SetLocation(defaultLocation);
		lastSpawned->SetActorLocation(defaultLocation);
		RemoveMyselfAsOwner(lastSpawned);
	}
}

bool AAstrumCharacter::MoveRelicToDefault_Validate()
{
	return true;
}

void AAstrumCharacter::GoToServer()
{
	owner->GoToServer();
}

bool AAstrumCharacter::ApplyUpgrades()
{
	FVector currentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();

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
		if (actor && actor != lastSpawned) { // check if spawnable actor
			TMap<FString, EAction> pct = actor->possibleCombinationTypes;
			for (auto combo : pct)
			{
				if (combo.Key == lastSpawned->category)
				{
					FRelicState rs;
					rs.relic = lastSpawned;
					rs.state = ERelicProcess::PRE;
					AddToActorCombos(actor, rs);
					return true;
				}
			}
		}

	}
	return false;
}

void AAstrumCharacter::AddToActorCombos_Implementation(ASpawnableActor* actor, FRelicState rs)
{
	if (actor->GetUserID() == owner->GetUserID()) {
		actor->SetOwner(owner);
		actor->AddCombinationRelic(rs);
		RemoveMyselfAsOwner(actor);
	}
}

bool AAstrumCharacter::AddToActorCombos_Validate(ASpawnableActor* actor, FRelicState rs)
{
	return true;
}

void AAstrumCharacter::CheckForUpgrades()
{
	FVector currentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();

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
		if (actor && actor != lastSpawned) { // check if spawnable actor
			TMap<FString, EAction> pct = actor->possibleCombinationTypes;
			auto SpawnableWidget = Cast<USpawningWidget>(CurrentWidget);
			SpawnableWidget->SetValidCombos(pct);
			return;
		}

	}

	auto SpawnableWidget = Cast<USpawningWidget>(CurrentWidget);
	SpawnableWidget->ClearValidCombos();
}

void AAstrumCharacter::PutBackInInventory()
{
	if (lastSpawned && lastSpawned->server_selected) {
		auto sw = Cast<USpawningWidget>(CurrentWidget);
		if (sw) {
			//add to options through spatial api
			//FRelic r = lastSpawned->CreateRelicFromProperties();
			sw->button_to_bring_back = lastSpawned->id;
			sw->BringBackButton();
			sw->button_to_bring_back = FString(""); // wipe this out, TODO: better handle
			//sw->AddToOptions(r);
			ToggleInventory();
		}
	}
}

void AAstrumCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAstrumCharacter, lastSpawned);
}