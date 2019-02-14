// Fill out your copyright notice in the Description page of Project Settings.

#include "AstrumPlayerController.h"
#include "AstrumComponentView.h"

using namespace improbable;

void AAstrumPlayerController::SetProperties() {
	//manually set them for now
	FLand a;
	//a.min = FVector2D(650590-1000, 605530 - 1000);
	//a.max = FVector2D(650590 + 1000, 605530 + 1000);
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

	UWorld* World = GetWorld();
	NetDriver = Cast<USpatialNetDriver>(World->GetNetDriver());
	//NetDriver->Dispatcher->ProcessedOps.AddDynamic(this, &AAstrumPlayerController::GetProcessOps);
	ComponentView = NewObject<UAstrumComponentView>();
}

void AAstrumPlayerController::GetProcessOps(FOpList OpList)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("here"));
	//USpatialStaticComponentView* StaticComponentView = NetDriver->StaticComponentView;
	Worker_OpList* ops = OpList.ops;
	for (size_t i = 0; i < ops->op_count; ++i)
	{
		Worker_Op* Op = &ops->ops[i];
		switch (Op->op_type)
		{
			// Entity Lifetime
		case WORKER_OP_TYPE_ADD_ENTITY:
			break;
		case WORKER_OP_TYPE_REMOVE_ENTITY:
			ComponentView->OnRemoveEntity(Op->remove_entity);
			break;

			// Components
		case WORKER_OP_TYPE_ADD_COMPONENT:
			ComponentView->OnAddComponent(Op->add_component);
			break;
		case WORKER_OP_TYPE_COMPONENT_UPDATE:
			ComponentView->OnComponentUpdate(Op->component_update);
			break;
		default:
			break;
		}

		if (Op->op_type == WORKER_OP_TYPE_ADD_COMPONENT && Op->add_component.data.component_id == 200002) {
			//const Worker_AddEntityOp ac = Op->add_entity;
			//get the stuff out of op
			//TSharedPtr<improbable::DynamicComponent> Data;
			//Coordinates coord = ComponentView->GetComponentData<improbable::Position>(ac.entity_id)->Coords;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, coord.ToFVector(coord).ToString());


			//RelicItemComponent r;
			//r.RelicType = FString("hi");
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "here");

			RelicItemComponent* ric = ComponentView->GetComponentData<RelicItemComponent>(Op->add_component.entity_id);

			
			//Data = MakeShared<improbable::DynamicComponent>(ic->Get());
			//Schema_Object* ComponentObject = Schema_GetComponentDataFields(Data->schema_type);
			//FString temp = GetStringFromSchema(ComponentObject, 1);
			if(ric != nullptr)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(ric->RelicType));
		}

		/*if (ac.data.component_id == 53) {
			Data = MakeShared<improbable::DynamicComponent>(ac.data);
			Schema_Object* ComponentObject = Schema_GetComponentDataFields(Data->Data->schema_type);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, GetStringFromSchema(ComponentObject, 1));
		}*/
	}

}