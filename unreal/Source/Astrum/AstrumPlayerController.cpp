// Fill out your copyright notice in the Description page of Project Settings.

#include "AstrumPlayerController.h"

void AAstrumPlayerController::SetProperties() {
	//manually set them for now
	FLand a;
	a.min = FVector2D(-1000, -1000);
	a.max = FVector2D(1000, 1000);
	properties.Add(a);
}

FString AAstrumPlayerController::GetUserID() {
	return userID;
}

TArray<FLand> AAstrumPlayerController::GetProperties() {
	return properties;
}