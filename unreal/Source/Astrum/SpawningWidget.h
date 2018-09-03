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

USTRUCT(BlueprintType)
struct FRelic {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D *icon;
};

UCLASS()
class ASTRUM_API USpawningWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USpawningWidget(const FObjectInitializer& ObjectInitializer);
	bool Initialize() override;
	void ButtonClick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExtendedWidget")
	FString ExtendedWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "options")
	TArray<FRelic> options;

	UFUNCTION(BlueprintCallable)
	TArray<FRelic> GetOptions();

	UFUNCTION(BlueprintCallable)
	void AddToOptions(FRelic relic);
	
	
	
	
};
