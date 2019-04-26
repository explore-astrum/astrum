// Fill out your copyright notice in the Description page of Project Settings.

#include "AstrumGameModeBase.h"
#include "AstrumCharacter.h"
#include "SpawningWidget.h"


void AAstrumGameModeBase::HandleMatchHasStarted() //kick this init off from game mode for now
{
	Super::HandleMatchHasStarted();

	UWorld* World = GetWorld();
	FString CurrentMapName = World->GetMapName().Mid(GetWorld()->StreamingLevelsPrefix.Len());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, CurrentMapName);
	/*NetDriver = Cast<USpatialNetDriver>(GetWorld()->GetNetDriver());
	if (CurrentMapName == "Astrum" && NetDriver) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "got net driver");
		NetDriver->Dispatcher->ProcessedOps.AddDynamic(this, &AAstrumGameModeBase::GetProcessOps);
		ComponentView = NewObject<UAstrumComponentView>();
	}*/
	tcpConnection = World->SpawnActor<ATCPConnection>();
	tcpConnection->ProcessedActorSpawn.AddDynamic(this, &AAstrumGameModeBase::SpawnActor);
	tcpConnection->ProcessedChangeOwner.AddDynamic(this, &AAstrumGameModeBase::ChangeRelicOwner);
	tcpConnection->ProcessedChangeLocation.AddDynamic(this, &AAstrumGameModeBase::MoveRelic);

	if (!ObjectLibrary)
	{
		ObjectLibrary = UObjectLibrary::CreateLibrary(TSubclassOf<ASpawnableActor>(), false, GIsEditor);
		ObjectLibrary->AddToRoot();
	}

	int asset_amount = ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game/Relics"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("loading assets...") + FString::FromInt(asset_amount));
	ObjectLibrary->LoadAssetsFromAssetData();
}

void AAstrumGameModeBase::GetProcessOps(FOpList OpList)
{
		/*Worker_OpList* ops = OpList.ops;
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

				new_relic->id = FString::FromInt(1);
				new_relic->isPawn = true;
				new_relic->pawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/VehicleBP/Sedan/Sedan.Sedan_C"), nullptr, LOAD_None, nullptr);
			}
		}*/

}

void AAstrumGameModeBase::SpawnActor(int relic_type, FString relic_key)
{
	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);

	for (int32 i = 0; i < AssetDatas.Num(); ++i) {
		FAssetData& AssetData = AssetDatas[i];

		FString FoundType;
		AssetData.GetTagValue(FName(TEXT("relic_type")), FoundType);

		if (FCString::Atoi(*FoundType) == uint32(relic_type)) {
			//TArray<FSoftObjectPath> ItemsToStream;
			//actors_to_spawn.Add(AssetData);
			UBlueprint* loaded = Cast<UBlueprint>(AssetData.GetAsset()); //uobject auto-garbage collected
			ASpawnableActor* new_relic = GetWorld()->SpawnActor<ASpawnableActor>(loaded->GeneratedClass, FVector(5000, 5000, -50000) /*random location thats within reach of players*/, FRotator::ZeroRotator);
			new_relic->SetID(relic_key);
			new_relic->ProcessedLocationChange.AddDynamic(this, &AAstrumGameModeBase::UpdateRelicLocation);
			all_relics.Add(new_relic);
			//ItemsToStream.AddUnique(AssetData.ToSoftObjectPath());
			//StreamableManager.RequestAsyncLoad(ItemsToStream, FStreamableDelegate::CreateUObject(this, &AAstrumGameModeBase::AsyncSpawn));
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Spawning Relic ID: " + relic_key));
}

void AAstrumGameModeBase::MoveRelic(FString relic_key, FVector location)
{
	for (int i = 0; i < all_relics.Num(); i++) {
		ASpawnableActor* relic = all_relics[i];
		if (relic->GetID() == relic_key && !relic->GetIsPlaced()) {
			relic->SetLocation(location);
			relic->PlaceObject(true);
		}
	}
}

void AAstrumGameModeBase::AsyncSpawn()
{
	//for async
	if (actors_to_spawn.Num() > 0) {
		actors_to_spawn.RemoveAt(0);
	}
}

void AAstrumGameModeBase::ChangeRelicOwner(FString relic_key, FString relic_owner)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Changing Relic Owner: " + relic_key));

	bool found = false;

	FRelic relicForInventory;
	TArray<AActor*> FoundRelics;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnableActor::StaticClass(), FoundRelics);
	for (int i = 0; i < FoundRelics.Num(); i++) {
		ASpawnableActor* actor = Cast<ASpawnableActor>(FoundRelics[i]);
		if (actor) {
			if (actor->GetID() == relic_key) {
				actor->userid = relic_owner;
				relicForInventory = actor->CreateRelicFromProperties();
				found = true;
				break;
			}
		}
	}

	if (found) {
		TArray<AActor*> FoundPlayers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAstrumCharacter::StaticClass(), FoundPlayers);
		for (int i = 0; i < FoundPlayers.Num(); i++) {
			AAstrumCharacter* actor = Cast<AAstrumCharacter>(FoundPlayers[i]);
			if (actor) {
				if (actor->owner->GetUserID() == relic_owner) {
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString("found owner"));
					actor->PutInInventoryClient(relicForInventory);
					break;
				}
			}
		}
	}

	
}

void AAstrumGameModeBase::UpdateRelicLocation(FString relic_key, FVector location)
{
	tcpConnection->SendRelicLocation(relic_key, location);
}

void AAstrumGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	auto player = Cast<AAstrumPlayerController>(NewPlayer);
	auto character = Cast<AAstrumCharacter>(player->GetPawn());
	for (int i = 0; i < all_relics.Num(); i++) {
		ASpawnableActor* relic = all_relics[i];
		if (!relic->GetIsPlaced() && relic->userid == player->GetUserID()) {
			FRelic relicForInventory = relic->CreateRelicFromProperties();
			character->PutInInventoryClient(relicForInventory);
		}
	}
}