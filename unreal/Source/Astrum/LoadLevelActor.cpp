// Fill out your copyright notice in the Description page of Project Settings.

#include "LoadLevelActor.h"

ALoadLevelActor::ALoadLevelActor(const FObjectInitializer &fobj) : Super(fobj) {
	//SpawnedActors.Add(LoadedObj(FString("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"), FVector(0, 0, 300)));
}

void ALoadLevelActor::LoadObjects() {
	for (auto sa : SpawnedActors) {
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		auto actor = GetWorld()->SpawnActor<ASpawnableActor>(sa.translation, Rotation, SpawnInfo);
		actor->SetMesh(sa.filename);
		sa.actor = actor;
	}
}