// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Engine/LevelScriptActor.h"
#include "SpawnableActor.h"
#include "LoadLevelActor.generated.h"

/**
 * 
 */
class LoadedObj
{
public:
	//FString filename;
	//FVector translation;
	//float scale;
	//ASpawnableActor* actor;
	//LoadedObj(FString _filename, FVector _translation) : translation(_translation), filename(_filename) {}
};

UCLASS()
class ASTRUM_API ALoadLevelActor : public ALevelScriptActor
{
	GENERATED_UCLASS_BODY()
public:
	//TArray<LoadedObj> SpawnedActors;
	//UFUNCTION(BlueprintCallable)
	void LoadObjects();
protected:
};
