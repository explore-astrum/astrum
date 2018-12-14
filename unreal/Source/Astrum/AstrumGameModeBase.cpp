// Fill out your copyright notice in the Description page of Project Settings.

#include "AstrumGameModeBase.h"
#include "SpawnableActor.h"


void AAstrumGameModeBase::HandleMatchHasStarted() //kick this init off from game mode for now
{
	Super::HandleMatchHasStarted();
	NetDriver = Cast<USpatialNetDriver>(GetWorld()->GetNetDriver());
	NetDriver->Dispatcher->ProcessedOps.AddDynamic(this, &AAstrumGameModeBase::GetProcessOps);
	ComponentView = NewObject<UAstrumComponentView>();
}

void AAstrumGameModeBase::GetProcessOps(FOpList OpList)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "here");
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
				RelicItemComponent* ric = ComponentView->GetComponentData<RelicItemComponent>(Op->add_component.entity_id);
				if (ric == nullptr)
					return;
				
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(ric->RelicType));
				ASpawnableActor* new_relic = GetWorld()->SpawnActor<ASpawnableActor>(StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/FirstPersonBP/AI/Sofa/CarTry.CarTry_C"), nullptr, LOAD_None, nullptr));
				FString::FromInt(Op->add_component.entity_id);
				/* testing relics
				new_relic->id = FString::FromInt(1);
				new_relic->isPawn = true;
				new_relic->pawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/VehicleBP/Sedan/Sedan.Sedan_C"), nullptr, LOAD_None, nullptr);
				*/
			}
		}

}