// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AstrumPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ASTRUM_API AAstrumPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void InitPlayerState() override;
	
	
	
	
};
