// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AstrumPlayerController.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "SpawningWidget.h"
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
	virtual void MoveRelicToDefault();
	virtual void GoToServer();

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

	UFUNCTION(Client, Reliable)
	void PlaceObjectLocal();

	UFUNCTION(Client, Reliable)
	void AssignToLocalController(ASpawnableActor* actor);


	AAstrumPlayerController* owner;
	bool init = false;
	bool inventoryOn = false;
	float pickupDistance = 300.0;
	FVector defaultLocation = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, Replicated)
	ASpawnableActor* lastSpawned;

	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere)
	UUserWidget* CurrentWidget;

	
	
};
