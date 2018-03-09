// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterMovable.generated.h"

UCLASS()
class ASTRUM_API ACharacterMovable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterMovable();
	USphereComponent* sphere;
	UStaticMeshComponent* SphereVisual;

	UPROPERTY(EditAnywhere, Category = Mesh)
		UStaticMesh* SphereVisualAsset;

	ACharacter* MainCharacter;
	APlayerController* controller;
	bool selected;
	bool rotating;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
	void RotateX();

	
	
};
