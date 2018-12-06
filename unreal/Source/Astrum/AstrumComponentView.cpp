#include "AstrumComponentView.h"

void UAstrumComponentView::OnAddComponent(const Worker_AddComponentOp& Op)
{
	TUniquePtr<improbable::ComponentStorageBase> Data;
	switch (Op.data.component_id)
	{
	case 200002:
		Data = MakeUnique<improbable::ComponentStorage<RelicItemComponent>>(Op.data);
		break;
	default:
		return;
	}

	EntityComponentMap.FindOrAdd(Op.entity_id).FindOrAdd(Op.data.component_id) = std::move(Data);
}

void UAstrumComponentView::OnRemoveEntity(const Worker_RemoveEntityOp& Op)
{
	EntityComponentMap.Remove(Op.entity_id);
}

void UAstrumComponentView::OnComponentUpdate(const Worker_ComponentUpdateOp& Op)
{
	improbable::Component* Component = nullptr;

	switch (Op.update.component_id)
	{
	case 200002:
		Component = GetComponentData<RelicItemComponent>(Op.entity_id);
		break;
	default:
		return;
	}

	if (Component) {
		Component->ApplyComponentUpdate(Op.update);
	}
}
