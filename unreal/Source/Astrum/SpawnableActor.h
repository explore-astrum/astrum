// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "AstrumPlayerController.h"
#include "SpawnableActor.generated.h"

UCLASS()
class ASTRUM_API ASpawnableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnableActor();
	USphereComponent* sphere;
	UStaticMeshComponent* SphereVisual;

	UPROPERTY(EditAnywhere, Replicated, Category = Mesh)
	UStaticMesh* SphereVisualAsset;
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_ChangeMaterial)
	bool server_selected;
	UPROPERTY(Replicated)
	FString id;

	UPROPERTY(Replicated)
	UClass* pawnClass;
	UPROPERTY(Replicated)
	bool isPawn;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void PlaceObject();
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetID(const FString &_id);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetPawnClass(UClass* type);
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetIsPawn(const bool is_pawn);
	UFUNCTION(BlueprintCallable)
	bool GetIsPawn();
	UFUNCTION(BlueprintCallable)
	UClass* GetPawn();

	UFUNCTION(BlueprintCallable)
	void AssignToPlayer();

	ACharacter* MainCharacter;
	APlayerController* controller;
	bool selected;
	bool rotating;
	bool assigned = false;

	UFUNCTION(BlueprintCallable)
	bool isSelected();

	UFUNCTION()
	virtual void OnRep_ChangeMaterial();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetLocation(FVector location);

	UFUNCTION(NetMulticast, Unreliable)
	void SetLocationMulticast(FVector location);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetServerSelected(bool _selected);

	UFUNCTION(BlueprintCallable)
	void SetSelected(bool _selected);

	UFUNCTION(BlueprintCallable)
	bool GetServerSelected();

	UFUNCTION(BlueprintCallable)
	FString GetID();

	UFUNCTION(BlueprintCallable)
	void MakePlaceable(bool place);

	UPROPERTY()
	float last_seen_time = -1;
	UPROPERTY()
	FVector last_seen_location;
	UPROPERTY()
	FVector velocity;
	UPROPERTY()
	bool can_place;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RotateX();
	void PickUp();

	
	
};
