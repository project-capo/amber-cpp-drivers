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

amber::DriverMsg* LocationProxy::buildMsg(int iSynNum)
{
		amber::DriverMsg* msg = new amber::DriverMsg();
		msg->set_type(amber::DriverMsg::DATA);

		msg->SetExtension(amber::location_proto::get_location,true);
		msg->set_synnum(iSynNum);

	return msg;
}

void LocationProxy::buildSendMessage(amber::DriverHdr header, amber::DriverMsg* msg)
{
	int headerLen = header.ByteSize();
	int messageLen = msg->ByteSize();
	requestLocationLength =  2 + headerLen + 2 + messageLen;

	char* output = new char[requestLocationLength];

	output[0] = (char) ((headerLen >> 8) & 0xff);
	output[1] = (char) (headerLen & 0xff);

	header.SerializePartialToArray(&output[2],headerLen);

	output[2 + headerLen] = (char) ((messageLen >> 8) & 0xff);
	output[2 + headerLen + 1] = (char) (messageLen & 0xff);

	msg->SerializePartialToArray(&output[2 + headerLen + 2],messageLen);

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
