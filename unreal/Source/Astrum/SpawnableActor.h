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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProcessedLocationChange, FString, relic_key, FVector, location);

USTRUCT(BlueprintType)
struct FRelic {
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite)
		FString id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D *icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass *pawnClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isPawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass *blueprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString category;
};

UENUM(BlueprintType)
enum class EAction : uint8 
{	TURN_ON UMETA(DisplayName = "Turn On"),
	PAINT UMETA(DisplayName = "Paint"),
	EVOLVE UMETA(DisplayName = "Evolve"),
	CUSTOM UMETA(DisplayName = "Custom")
}; //example of action enums for each relic

UENUM(BlueprintType)
enum class ERelicProcess : uint8 
{	PRE UMETA(DisplayName = "Hasn't Started"),
	DURING UMETA(DisplayName = "In Process"),
	POST UMETA(DisplayName = "Done")
};

USTRUCT()
struct FRelicState {
	GENERATED_BODY()

	UPROPERTY()
	ASpawnableActor* relic;
	UPROPERTY()
	ERelicProcess state;
};

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
	UPROPERTY(EditAnywhere, Replicated)
	FString id;
	UPROPERTY(EditAnywhere, Replicated, AssetRegistrySearchable)
	FString relic_type;
	UPROPERTY(EditAnywhere, Replicated)
	FString userid;
	UPROPERTY(EditAnywhere)
	UTexture2D *icon;
	UPROPERTY(EditAnywhere)
	UClass *blueprint;

	UPROPERTY(EditAnywhere, Replicated)
	UClass* pawnClass;
	UPROPERTY(EditAnywhere, Replicated)
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

	UFUNCTION()
	virtual void OnRep_ChangeCombinations();

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
	FString GetUserID();

	UFUNCTION(BlueprintCallable)
	void MakePlaceable(bool place);

	//combined actions
	UFUNCTION()
	void TurnMeOn();

	UFUNCTION()
	void PaintMe(ASpawnableActor* actor);

	UFUNCTION()
	void EvolveMe(ASpawnableActor* actor);

	UFUNCTION()
	void StartEvolution();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CustomComboAction(ASpawnableActor* actor);

	UPROPERTY()
	float last_seen_time = -1;
	UPROPERTY()
	FVector last_seen_location;
	UPROPERTY()
	FVector velocity;
	UPROPERTY()
	bool can_place;


	UPROPERTY(EditAnywhere)
	TMap<FString, EAction> possibleCombinationTypes;
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_ChangeCombinations)
	TArray<FRelicState> combinedRelics;

	UPROPERTY(EditAnywhere, Replicated)
	FString category;
	UPROPERTY(Replicated)
	bool turnedOn = false; // can use pawn

	UFUNCTION()
	void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION(Server, Reliable, WithValidation)
	void AddCombinationRelic(FRelicState relicState);

	UFUNCTION()
	FRelic CreateRelicFromProperties();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RotateX();
	void PickUp();

	FProcessedLocationChange ProcessedLocationChange;

	
	
};
