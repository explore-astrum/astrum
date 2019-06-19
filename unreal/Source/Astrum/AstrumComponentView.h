#pragma once

#include "CoreMinimal.h"

//#include "Interop/SpatialStaticComponentView.h"
//#include "EngineClasses/SpatialGameInstance.h"
//#include "EngineClasses/SpatialNetDriver.h"
//#include "Schema/Component.h"
//#include "Schema/DynamicComponent.h"
//#include "Schema/StandardLibrary.h"
//#include "Interop/SpatialDispatcher.h"

//#include <WorkerSDK/improbable/c_schema.h>
//#include <WorkerSDK/improbable/c_worker.h>

#include "AstrumComponentView.generated.h"

//using namespace improbable;

//struct RelicItemComponent : Component
//{
//	static const Worker_ComponentId ComponentId = 200002;
//
//	RelicItemComponent() = default;
//
//	RelicItemComponent(const Worker_ComponentData& Data)
//	{
//		Schema_Object* ComponentObject = Schema_GetComponentDataFields(Data.schema_type);
//		RelicType = Schema_GetInt64(ComponentObject, 1);
//	}
//
//	uint64 RelicType;
//};

UCLASS()
class ASTRUM_API UAstrumComponentView : public UObject
{
	GENERATED_BODY()

public:
	//template <typename T>
	/*T* GetComponentData(Worker_EntityId EntityId)
	{
		if (TMap<Worker_ComponentId, TUniquePtr<improbable::ComponentStorageBase>>* ComponentStorageMap = EntityComponentMap.Find(EntityId))
		{
			if (TUniquePtr<improbable::ComponentStorageBase>* Component = ComponentStorageMap->Find(T::ComponentId))
			{
				return &(static_cast<improbable::ComponentStorage<T>*>(Component->Get())->Get());
			}
		}

		return nullptr;
	}

	void OnAddComponent(const Worker_AddComponentOp& Op);
	void OnRemoveEntity(const Worker_RemoveEntityOp& Op);
	void OnComponentUpdate(const Worker_ComponentUpdateOp& Op);*/

private:
	//TMap<Worker_EntityId_Key, TMap<Worker_ComponentId, TUniquePtr<improbable::ComponentStorageBase>>> EntityComponentMap;
};
