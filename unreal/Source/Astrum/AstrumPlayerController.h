// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AstrumPlayerController.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FLand {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D min;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D max;
};

UCLASS(SpatialType)
class ASTRUM_API AAstrumPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:
		UPROPERTY(EditAnywhere)
		FString userID = "1";

		UFUNCTION(BlueprintCallable)
		FString GetUserID();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "properties")
		TArray<FLand> properties;

		UFUNCTION(BlueprintCallable)
		void SetProperties();

		UFUNCTION(BlueprintCallable)
		TArray<FLand> GetProperties();
	
	
	
	
};
