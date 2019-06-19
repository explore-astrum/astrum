// Fill out your copyright notice in the Description page of Project Settings.

#include "AstrumGameInstance.h"
#include "MoviePlayer.h"
#include "Misc/CoreDelegates.h"


void UAstrumGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UAstrumGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UAstrumGameInstance::EndLoadingScreen);
	//FCoreDelegates::OnAsyncLoadingFlush.AddUObject(this, &UAstrumGameInstance::EndLoadingScreen);
	//FCoreDelegates::OnAsyncLoadingFlushUpdate.AddUObject(this, &UAstrumGameInstance::EndLoadingScreen);
}

void UAstrumGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer() && MapName == "/Game/FirstPersonBP/Maps/Astrum")
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		//LoadingScreen.bWaitForManualStop = true;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 10.0f;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
		//GetMoviePlayer()->PlayMovie();

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, MapName);
	}
}

void UAstrumGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	//GetMoviePlayer()->StopMovie();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "fffffhereeeeeeeeeee dydeteee");
}