// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawningWidget.h"


USpawningWidget::USpawningWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	FRelic a;
	a.mesh = FString("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere");
	a.material = FString("/Game/FirstPersonBP/Blueprints/ObjectTestMaterial.ObjectTestMaterial");
	a.icon = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("/Game/FirstPersonBP/Maps/MapTextures/Desert_Rock_Albedo.Desert_Rock_Albedo")).Object;
	options.Add(a);

	FRelic b;
	b.mesh = FString("/Game/Sky/Meshes/SM_RotatorCube.SM_RotatorCube");
	b.material = FString("/Game/FirstPersonBP/Blueprints/ObjectTestMaterial.ObjectTestMaterial");
	b.icon = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("/Game/FirstPersonBP/Maps/MapTextures/Beach_Sand_Normal.Beach_Sand_Normal")).Object;
	options.Add(b);
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

void USpawningWidget::ButtonClick() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat(0.0));
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	auto actor = GetWorld()->SpawnActor<ASpawnableActor>(Location, Rotation, SpawnInfo);
}

TArray<FRelic> USpawningWidget::GetOptions() {
	FRelic c;
	c.mesh = FString("/Game/Sky/Meshes/SM_RotatorCube.SM_RotatorCube");
	c.material = FString("/Game/FirstPersonBP/Blueprints/ObjectTestMaterial.ObjectTestMaterial");
	c.icon = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/FirstPersonBP/Maps/MapTextures/Beach_Sand_Normal.Beach_Sand_Normal")));
	options.Add(c);
	return options;
}
