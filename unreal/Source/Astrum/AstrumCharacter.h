// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AstrumPlayerController.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "SpawningWidget.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "TCPConnection.h"
#include "AstrumCharacter.generated.h"

UCLASS()
class ASTRUM_API AAstrumCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAstrumCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetupInventory();
	virtual void CreateMyWidget();
	virtual void MovePlayerToStartingPosition();
	virtual void ToggleInventory();
	virtual void ScrollLeft();
	virtual void ScrollRight();
	virtual void CheckForObjectHit();
	virtual void PutActorDown(ASpawnableActor* actor);

	UFUNCTION(BlueprintCallable)
	virtual void GoToServer();
	virtual void CheckMusic();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	virtual void ServerChangeOwner(ASpawnableActor* actor);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	virtual void ReplacePawn(ASpawnableActor* actor);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetCharacterLocation(FVector location);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void RemoveMyselfAsOwner(ASpawnableActor* actor);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void MoveRelicToMe(UClass* actorType, const FString &relicID);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	virtual void MoveRelicToDefault();

	UFUNCTION(Client, Reliable)
	void PlaceObjectLocal();

	UFUNCTION(Client, Reliable)
	void AssignToLocalController(ASpawnableActor* actor);

	UFUNCTION(Server, Reliable, WithValidation)
	void AddToActorCombos(ASpawnableActor* actor, FRelicState rs);

	UFUNCTION()
	void CheckForUpgrades();

	UFUNCTION()
	bool ApplyUpgrades();

	UFUNCTION()
	void PutBackInInventory();

	UFUNCTION()
	void UpdateInventory();

	UFUNCTION(Client, Reliable)
	void PutInInventoryClient(FRelic relic);


	AAstrumPlayerController* owner;
	bool init = false;
	bool inventoryOn = false;
	float pickupDistance = 300.0;
	FVector defaultLocation = FVector(5000, 5000, -40000);

	UPROPERTY(EditAnywhere, Replicated)
	ASpawnableActor* lastSpawned;

	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere)
	UUserWidget* CurrentWidget;

	TArray<UAudioComponent*> songs;
	TArray<USoundCue*> soundCues;

	int ind = 0;
	int indx = 0;
	bool move_on = true;
	int last_timey = 0;
	int numticks = 0;
	int numrots = 0;

	TArray<FRelic> pendingInventory;

	bool moved_player = false;

	
	
};
