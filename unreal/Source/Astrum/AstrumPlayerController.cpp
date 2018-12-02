// Fill out your copyright notice in the Description page of Project Settings.

#include "AstrumPlayerController.h"

using namespace improbable;

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

void AAstrumPlayerController::GoToServer() {
	FString TravelURL = TEXT("127.0.0.1:7777/Astrum");
	ClientTravel(TravelURL, TRAVEL_Absolute, false /*bSeamless*/);
}

void AAstrumPlayerController::BeginPlay()
{
	Super::BeginPlay();
	NetDriver = Cast<USpatialNetDriver>(GetWorld()->GetNetDriver());
	NetDriver->Dispatcher->ProcessedOps.AddDynamic(this, &AAstrumPlayerController::GetProcessOps);
}

void AAstrumPlayerController::GetProcessOps(FOpList OpList)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("here"));
	USpatialStaticComponentView* StaticComponentView = NetDriver->StaticComponentView;
	Worker_OpList* ops = OpList.ops;
	for (size_t i = 0; i < ops->op_count; ++i)
	{
		Worker_Op* Op = &ops->ops[i];
		if (Op->op_type == WORKER_OP_TYPE_ADD_ENTITY) {
			const Worker_AddEntityOp ac = Op->add_entity;
			//get the stuff out of op
			//TSharedPtr<improbable::DynamicComponent> Data;
			Coordinates coord = StaticComponentView->GetComponentData<improbable::Position>(ac.entity_id)->Coords;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, coord.ToFVector(coord).ToString());
		}

		/*if (ac.data.component_id == 53) {
			Data = MakeShared<improbable::DynamicComponent>(ac.data);
			Schema_Object* ComponentObject = Schema_GetComponentDataFields(Data->Data->schema_type);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, GetStringFromSchema(ComponentObject, 1));
		}*/
	}

}