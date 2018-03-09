// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawningWidget.h"


USpawningWidget::USpawningWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	FRelic a;
	a.mesh = FString("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere");
	a.material = FString("/Game/StarterContent/Materials/M_CobbleStone_Pebble");
	options.Add(a);

	FRelic b;
	b.mesh = FString("/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus");
	b.material = FString("/Game/StarterContent/Materials/M_Ground_Grass");
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