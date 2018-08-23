// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnableActor.generated.h"

UCLASS(SpatialType)
class ASTRUM_API ASpawnableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnableActor();
	USphereComponent* sphere;
	UStaticMeshComponent* SphereVisual;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_SetMesh, Category = Mesh)
	UStaticMesh* SphereVisualAsset;
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_SetMaterial)
	UMaterial* Material;

	void SetMesh(int type);
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetMesh(const FString &type);
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetIntermediateMaterial();
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SetMaterial(const FString &type);

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
	virtual void OnRep_SetMaterial();
	UFUNCTION()
	virtual void OnRep_SetMesh();

	UFUNCTION(Server, Reliable, WithValidation)
	void SetLocation(FVector location);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RotateX();
	void PickUp();

	
	
};
