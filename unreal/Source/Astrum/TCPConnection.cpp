// Fill out your copyright notice in the Description page of Project Settings.

#include <string>
#include "TCPConnection.h"

uint16 deserialize_uint16(unsigned char *buf)
{
	uint16 *x = (uint16*)buf;
	return *x;
}


uint32 deserialize_uint32(unsigned char *buf)
{
	uint32 *x = (uint32*)buf;
	return *x;
}

ATCPConnection::ATCPConnection()
{
	PrimaryActorTick.bCanEverTick = true;
}

ATCPConnection::~ATCPConnection()
{
}

void ATCPConnection::BeginPlay()
{
	Super::BeginPlay();
	Launch();
}


void ATCPConnection::Launch()
{
	if (!StartTCPReceiver("RamaSocketListener", "127.0.0.1", 16000))
	{
		return;
	}

}

bool ATCPConnection::StartTCPReceiver(
	const FString& YourChosenSocketName,
	const FString& TheIP,
	const int32 ThePort
) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("HERE"));
	ListenerSocket = CreateTCPConnectionListener(YourChosenSocketName, TheIP, ThePort);

	if (!ListenerSocket)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("StartTCPReceiver>> Listen socket could not be created! ~> %s %d"), *TheIP, ThePort));
		return false;
	}

	FTimerHandle timerhandle;
	GetWorld()->GetTimerManager().SetTimer(timerhandle, this, &ATCPConnection::TCPConnectionListener, 0.01);

	return true;
}

bool ATCPConnection::FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4])
{
	//IP Formatting
	TheIP.Replace(TEXT(" "), TEXT(""));

	TArray<FString> Parts;
	TheIP.ParseIntoArray(Parts, TEXT("."), true);
	if (Parts.Num() != 4)
		return false;

	//String to Number Parts
	for (int32 i = 0; i < 4; ++i)
	{
		Out[i] = FCString::Atoi(*Parts[i]);
	}

	return true;
}

FSocket* ATCPConnection::CreateTCPConnectionListener(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, const int32 ReceiveBufferSize)
{
	uint8 IP4Nums[4];
	if (!FormatIP4ToNumber(TheIP, IP4Nums))
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Yellow, "Invalid IP! Expecting 4 parts separated by .");
		return false;
	}

	FIPv4Address ip(127, 0, 0, 1);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(16000);
	FSocket* ListenSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	ListenSocket->Connect(*addr);

	int32 NewSize = 0;
	ListenSocket->SetReceiveBufferSize(ReceiveBufferSize, NewSize);
	return ListenSocket;
}

void ATCPConnection::TCPConnectionListener()
{
	if (!ListenerSocket) return;

	FTimerHandle timerhandle;
	GetWorld()->GetTimerManager().SetTimer(timerhandle, this, &ATCPConnection::TCPSocketListener, 0.01);
}

FString ATCPConnection::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	BinaryArray.Add(0);
	FString BrokenString = BytesToString(BinaryArray.GetData(), BinaryArray.Num());
	FString Fixed;

	for (int i = 0; i < BrokenString.Len(); i++)
	{
		const TCHAR c = BrokenString[i] - 1;
		Fixed.AppendChar(c);
	}

	return Fixed;
}

TArray<uint8> ATCPConnection::ReadOutBinary(uint32 messageSize) {
	TArray<uint8> ReceivedData;
	ReceivedData.Init(0, messageSize);

	int32 Read = 0;
	ListenerSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
	return ReceivedData;
}

void ATCPConnection::TCPSocketListener()
{
	if (!ListenerSocket) return;

	uint32 Size;
	while (ListenerSocket->HasPendingData(Size)) {
		uint32 messagetype_size = 2;
		TArray<uint8> messageTypeBinary = ReadOutBinary(messagetype_size);

		if (messageTypeBinary.Num() <= 0)
			return;

		int message_type = deserialize_uint16(reinterpret_cast<unsigned char*>(messageTypeBinary.GetData()));
		if (message_type == 0) {
			uint32 message_length = 4;
			TArray<uint8> relicTypeBinary = ReadOutBinary(4);
			if (relicTypeBinary.Num() <= 0)
				return;

			int relic_type = deserialize_uint32(reinterpret_cast<unsigned char*>(relicTypeBinary.GetData()));
			FString relic_type_str = FString(std::to_string(relic_type).c_str());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Relic Type: " + relic_type_str));

			TArray<uint8> relicKeyBinary = ReadOutBinary(16);
			if (relicKeyBinary.Num() <= 0)
				return;

			FString relic_key = StringFromBinaryArray(relicKeyBinary);

			ProcessedActorSpawn.Broadcast(relic_type, relic_key);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Relic Type: " + relic_key));

			//new line
			ReadOutBinary(1);
		}

	}
	FTimerHandle timerhandle;
	GetWorld()->GetTimerManager().SetTimer(timerhandle, this, &ATCPConnection::TCPSocketListener, 0.01);
}