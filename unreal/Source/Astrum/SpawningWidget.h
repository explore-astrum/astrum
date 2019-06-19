// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "SpawnableActor.h"
#include "SpawningWidget.generated.h"

/**
 * 
 */

UCLASS()
class ASTRUM_API USpawningWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USpawningWidget(const FObjectInitializer& ObjectInitializer);
	bool Initialize() override;
	//void ButtonClick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExtendedWidget")
	FString ExtendedWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "options")
	TArray<FRelic> options;

	UFUNCTION(BlueprintCallable)
	TArray<FRelic> GetOptions();

	UFUNCTION(BlueprintCallable)
	void AddToOptions(FRelic relic);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GetNewOptions();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ScrollLeft();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ScrollRight();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleInventory();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlaceObject();

	UFUNCTION(BlueprintCallable)
	void SetValidCombos(TMap<FString, EAction> pct);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowValidCombos();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ClearValidCombos();

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> possibleCombos;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BringBackButton();

	UPROPERTY(EditAnywhere)
	FString button_to_bring_back;

	UFUNCTION(BlueprintCallable)
	FString GetButtonToBringBack();
	
	
	
};
