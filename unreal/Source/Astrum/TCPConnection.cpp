// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPConnection.h"
#include <string>

uint16 deserialize_uint16(unsigned char *buf)
{
	//uint16 *x = (uint16*)buf;
	return (buf[0] << 8) + buf[1];
}

int deserialize_int(unsigned char *buf)
{
	int16 *x = (int16*)buf;
	return *x;
}


uint32 deserialize_uint32(unsigned char *buf)
{
	uint32 *x = (uint32*)buf;
	return *x;
}

uint8* serialize_float(float &f)
{
	uint8 *bytef = reinterpret_cast<uint8*>(&f);
	return bytef;
}

float deserialize_float(unsigned char *buf)
{
	float f1 = *(float*)buf;
	
	float retf;
	char *floatToConvert = (char*)& f1;
	char *returnFloat = (char*)& retf;

	returnFloat[0] = floatToConvert[3];
	returnFloat[1] = floatToConvert[2];
	returnFloat[2] = floatToConvert[1];
	returnFloat[3] = floatToConvert[0];

	return retf;
}

uint8* serialize_int16(uint16 i)
{
	static uint8 ibytes[2];
	ibytes[1] = (uint8)(i >> 8);
	ibytes[0] = (uint8)i;
	return ibytes;
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
}

void ATCPConnection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!init) {
		Launch();
		init = true;
	}
}


void ATCPConnection::Launch()
{
	if (!StartTCPReceiver("RamaSocketListener", "astrum.ironbay.co", 32000))
	{
		return;
	}

}

bool ATCPConnection::StartTCPReceiver(
	const FString& YourChosenSocketName,
	const FString& TheIP,
	const int32 ThePort
) {
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
	//uint8 IP4Nums[4];
	/*if (!FormatIP4ToNumber(TheIP, IP4Nums))
	{
		GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Yellow, "Invalid IP! Expecting 4 parts separated by .");
		return false;
	}*/

	//FIPv4Address ip(127, 0, 0, 1);

	auto ResolveInfo = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetHostByName("astrum.ironbay.co");
    while (!ResolveInfo->IsComplete());

	if (ResolveInfo->GetErrorCode() != 0)
		return false;

	const FInternetAddr* addr_temp = &ResolveInfo->GetResolvedAddress();
	TSharedPtr<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetRawIp(addr_temp->GetRawIp());
	addr->SetPort(32000);
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

		uint16 message_type = deserialize_uint16(reinterpret_cast<unsigned char*>(messageTypeBinary.GetData()));
		switch (message_type) {
			case 0: {
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
				break;
			}
			case 1: {
				TArray<uint8> relicKeyBinary = ReadOutBinary(16);
				if (relicKeyBinary.Num() <= 0)
					return;

				FString relic_key = StringFromBinaryArray(relicKeyBinary);

				TArray<uint8> relicOwnerBinary = ReadOutBinary(20);
				if (relicOwnerBinary.Num() <= 0)
					return;

				FString relic_owner = StringFromBinaryArray(relicOwnerBinary);

				ProcessedChangeOwner.Broadcast(relic_key, relic_owner);

				//new line
				ReadOutBinary(1);
				break;
			}
			case 2: {
				//ReadOutBinary(29);

				TArray<uint8> relicKeyBinary = ReadOutBinary(16);
				if (relicKeyBinary.Num() <= 0)
					return;

				FString relic_key = StringFromBinaryArray(relicKeyBinary);

				TArray<uint8> xBinary = ReadOutBinary(4);
				float x = deserialize_float(reinterpret_cast<unsigned char*>(xBinary.GetData()));

				TArray<uint8> yBinary = ReadOutBinary(4);
				float y = deserialize_float(reinterpret_cast<unsigned char*>(yBinary.GetData()));

				TArray<uint8> zBinary = ReadOutBinary(4);
				float z = deserialize_float(reinterpret_cast<unsigned char*>(zBinary.GetData()));

				ProcessedChangeLocation.Broadcast(relic_key, FVector(x, y, z));

				ReadOutBinary(1);
				break;
			}
			case 5: {
				TArray<uint8> xBinary = ReadOutBinary(2);
				int x = deserialize_int(reinterpret_cast<unsigned char*>(xBinary.GetData()));
				TArray<uint8> yBinary = ReadOutBinary(2);
				int y = deserialize_int(reinterpret_cast<unsigned char*>(yBinary.GetData()));

				TArray<uint8> plotBinary = ReadOutBinary(8);
				FString plot_id = StringFromBinaryArray(plotBinary);

				ProcessedAddPlot.Broadcast(plot_id, FVector2D(x, y));

				ReadOutBinary(1);
				break;
			}
			case 6: {
				TArray<uint8> plotBinary = ReadOutBinary(8);
				FString plot_id = StringFromBinaryArray(plotBinary);
				TArray<uint8> ownerBinary = ReadOutBinary(20);
				FString owner = StringFromBinaryArray(ownerBinary);

				ProcessedChangePlotOwner.Broadcast(plot_id, owner);
				ReadOutBinary(1);
				break;
			}

		}

	}
	FTimerHandle timerhandle;
	GetWorld()->GetTimerManager().SetTimer(timerhandle, this, &ATCPConnection::TCPSocketListener, 0.01);
}

void ATCPConnection::SendRelicLocation(FString relic_key, FVector location)
{
	uint16 type = 2;
	uint8 *serializedChar = (uint8*)TCHAR_TO_UTF8(relic_key.GetCharArray().GetData());
	uint8 *f1 = serialize_float(location.X);
	uint8 *f2 = serialize_float(location.Y);
	uint8 *f3 = serialize_float(location.Z);

	static uint8 to_send[31];

	uint8 *typeArr = serialize_int16(type);
	for (int i = 0; i < 2; i++) {
		to_send[i] = typeArr[1 - i];
	}
	for (int i = 0; i < 16; i++) {
		to_send[i + 2] = serializedChar[i];
	}
	for (int i = 0; i < 4; i++) {
		to_send[i + 18] = f1[3 - i];
	}
	for (int i = 0; i < 4; i++) {
		to_send[i + 22] = f2[3 - i];
	}
	for (int i = 0; i < 4; i++) {
		to_send[i + 26] = f3[3 - i];
	}

	uint8 *newlineChar = (uint8*)TCHAR_TO_UTF8(FString("\n").GetCharArray().GetData());

	to_send[30] = newlineChar[0];

	int32 size = 31;
	int32 sent = 0;
	bool successful = ListenerSocket->Send(to_send, size, sent);

}