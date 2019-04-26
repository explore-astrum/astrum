// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "TCPConnection.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProcessedActorSpawn, int, type, FString, relic);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProcessedChangeOwner, FString, relic, FString, owner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProcessedChangeLocation, FString, relic, FVector, location);

UCLASS()
class ASTRUM_API ATCPConnection : public AActor
{
	GENERATED_BODY()
public:
	ATCPConnection();
	~ATCPConnection();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FSocket* ListenerSocket;
	FSocket* ConnectionSocket;
	FIPv4Endpoint RemoteAddressForConnection;

	void Launch();

	bool StartTCPReceiver(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort
	);

	FSocket* CreateTCPConnectionListener(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort,
		const int32 ReceiveBufferSize = 2 * 1024 * 1024
	);

	TArray<uint8> ReadOutBinary(uint32 messageSize);

	//Timer functions, could be threads
	void TCPConnectionListener(); 	//can thread this eventually
	void TCPSocketListener();		//can thread this eventually


	//Format String IP4 to number array
	bool FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4]);

	//Rama's StringFromBinaryArray
	FString StringFromBinaryArray(const TArray<uint8> BinaryArray);
	void SendRelicLocation(FString relic_key, FVector location);

	FProcessedActorSpawn ProcessedActorSpawn;
	FProcessedChangeOwner ProcessedChangeOwner;
	FProcessedChangeLocation ProcessedChangeLocation;

	bool init = false;
};

	

	

	