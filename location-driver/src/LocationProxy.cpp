#include "UdpClientLinux.h"
#include "LocationProxy.h"

LocationProxy::LocationProxy(UdpClient *client_udp)
{
	udp = client_udp;

	synNum = 100;

	amber::DriverHdr hdr = buildHeader();
	message = buildMsg(synNum);

	buildSendMessage(hdr,message);
}

amber::DriverHdr  LocationProxy::buildHeader()
{
	amber::DriverHdr driverHdrBuilder;
	driverHdrBuilder.set_devicetype(DEVICE_TYPE);
	driverHdrBuilder.set_deviceid(DEVICE_ID);

	return driverHdrBuilder;
}

amber::DriverMsg* LocationProxy::buildMsg(int synNum)
{
		amber::DriverMsg* message = new amber::DriverMsg();
		message->set_type(amber::DriverMsg::DATA);

		message->SetExtension(amber::location_proto::get_location,true);
		message->set_synnum(synNum);

	return message;
}

void LocationProxy::buildSendMessage(amber::DriverHdr header, amber::DriverMsg* message)
{
	int headerLen = header.ByteSize();
	int messageLen = message->ByteSize();
	requestLocationLength =  2 + headerLen + 2 + messageLen;

	char* output = new char[requestLocationLength];

	output[0] = ((headerLen >> 8) & 0xff);
	output[1] = (headerLen & 0xff);

	header.SerializePartialToArray(&output[2],headerLen);

	output[2 + headerLen] = ((messageLen >> 8) & 0xff);
	output[2 + headerLen + 1] = (messageLen & 0xff);

	message->SerializePartialToArray(&output[2 + headerLen + 2],messageLen);

	requestLocation = output;
}

LocationProxy::~LocationProxy()
{
	delete [] requestLocation;
	delete message;
}

double LocationProxy::X()
{
	return 	pos_X;
}

double LocationProxy::Y()
{
	return pos_Y;
}

void LocationProxy::GetLocation()
{
	char *packetBytes;
	amber::location_proto::Location* currentLocation;

	udp->Send(requestLocation,requestLocationLength);
	packetBytes = udp->Receive();

	currentLocation = locationRequest(packetBytes);

	pos_X =  currentLocation->x();
	pos_Y = currentLocation->y();
}

amber::location_proto::Location* LocationProxy::locationRequest(char* packetBytes)
{
	int headerLen = (packetBytes[0] << 8) + (packetBytes[1] & 0xff);
	int messageLen = (packetBytes[2 + headerLen] << 8) + (packetBytes[2 + headerLen + 1] & 0xff);
	const char* shiftPacketBytes = &(packetBytes[2 + headerLen + 2]);

	message->ParseFromArray(shiftPacketBytes,messageLen);

	return message->MutableExtension(amber::location_proto::currentLocation);
}
