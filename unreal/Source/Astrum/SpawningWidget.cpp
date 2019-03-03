// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawningWidget.h"


USpawningWidget::USpawningWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	/*FRelic a;
	a.id = "1";
	a.mesh = FString("/Game/Flowers_Plants_Part2/Meshes/SM_Cannabis_01");
	a.material = FString("/Game/FirstPersonBP/Blueprints/ObjectTestMaterial.ObjectTestMaterial");
	a.icon = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("/Game/FirstPersonBP/Maps/MapTextures/Desert_Rock_Albedo.Desert_Rock_Albedo")).Object;
	a.isPawn = false;
	a.pawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/VehicleBP/Sedan/Sedan.Sedan_C"), nullptr, LOAD_None, nullptr);
	options.Add(a);
	/*
	FRelic b;
	b.id = "1";
	b.mesh = FString("/Game/Sky/Meshes/SM_RotatorCube.SM_RotatorCube");
	b.material = FString("/Game/FirstPersonBP/Blueprints/ObjectTestMaterial.ObjectTestMaterial");
	b.icon = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("/Game/FirstPersonBP/Maps/MapTextures/Beach_Sand_Normal.Beach_Sand_Normal")).Object;
	b.isPawn = false;
	b.pawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/VehicleBP/Sedan/Sedan.Sedan_C"), nullptr, LOAD_None, nullptr);
	options.Add(b);*/

	FRelic c;
	c.id = "1";
	c.icon = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("/Game/FirstPersonBP/Maps/MapTextures/Beach_Sand_Normal.Beach_Sand_Normal")).Object;
	c.isPawn = false;
	c.pawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/FirstPersonBP/AI/Sofa/SofaTry.SofaTry_C"), nullptr, LOAD_None, nullptr);
	c.blueprint = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/FirstPersonBP/AI/Sofa/SofaTry.SofaTry_C"), nullptr, LOAD_None, nullptr);
	c.category = "thing";
	options.Add(c);

	FRelic d;
	d.id = "2";
	d.icon = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("/Game/FirstPersonBP/Maps/MapTextures/Beach_Sand_Normal.Beach_Sand_Normal")).Object;
	d.isPawn = false;
	d.pawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/VehicleBP/Sedan/Sedan.Sedan_C"), nullptr, LOAD_None, nullptr);
	d.blueprint = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/FirstPersonBP/AI/Sofa/PaintTry.PaintTry_C"), nullptr, LOAD_None, nullptr);
	d.category = "paint";
	options.Add(d);
}
bool USpawningWidget::Initialize() {
	bool b = Super::Initialize();
	//auto MyCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
	//auto MyButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());

	//MyButton->OnClicked.AddDynamic(this, &USpawningWidget::ButtonClick);
	//MyCanvas->AddChildToCanvas(MyButton);
	//WidgetTree->RootWidget = MyCanvas;

	return b;
}

/*void USpawningWidget::ButtonClick() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat(0.0));
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	auto actor = GetWorld()->SpawnActor<ASpawnableActor>(Location, Rotation, SpawnInfo);
}*/

TArray<FRelic> USpawningWidget::GetOptions() {
	return options;
}

void USpawningWidget::AddToOptions(FRelic relic) {
	options.Add(relic);
}

void USpawningWidget::SetValidCombos(TMap<FString, EAction> pct) {
	possibleCombos.Empty();
	for (auto combo : pct)
	{
		possibleCombos.Add(combo.Key);
	}
	ShowValidCombos();
}

FString USpawningWidget::GetButtonToBringBack() {
	return button_to_bring_back;
}
