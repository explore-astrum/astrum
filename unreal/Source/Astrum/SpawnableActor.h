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

	UPROPERTY(EditAnywhere, Category = Mesh)
	UStaticMesh* SphereVisualAsset;
	//UPROPERTY(Replicated)
	UMaterial* Material;

	void SetMesh(int type);
	UFUNCTION(BlueprintCallable)
	void SetMesh(FString type);
	UFUNCTION(BlueprintCallable)
	void SetIntermediateMaterial();
	UFUNCTION(BlueprintCallable)
	void SetMaterial(FString type);

	ACharacter* MainCharacter;
	APlayerController* controller;
	bool selected;
	bool rotating;

	UFUNCTION(BlueprintCallable)
	bool isSelected();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RotateX();
	void PickUp();

	
	
};
