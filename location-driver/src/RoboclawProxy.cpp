#include "UdpClientLinux.h"
#include "RoboclawProxy.h"

RoboclawProxy::RoboclawProxy(UdpClient *client_udp)
{
	udp = client_udp;

	synNum = 100;
	wheelTrack = 0.280;

	amber::DriverHdr hdr = buildHeader();
	message = buildMsg(synNum);

	buildSendMessage(hdr,message);
}

amber::DriverHdr  RoboclawProxy::buildHeader()
{
	amber::DriverHdr driverHdrBuilder;
	driverHdrBuilder.set_devicetype(DEVICE_TYPE);
	driverHdrBuilder.set_deviceid(DEVICE_ID);

	return driverHdrBuilder;
}

amber::DriverMsg* RoboclawProxy::buildMsg(int synNum)
{
		amber::DriverMsg* message = new amber::DriverMsg();
		message->set_type(amber::DriverMsg::DATA);

		message->SetExtension(amber::roboclaw_proto::currentSpeedRequest,true);
		message->set_synnum(synNum);

	return message;
}

void RoboclawProxy::buildSendMessage(amber::DriverHdr header, amber::DriverMsg* message)
{
	int headerLen = header.ByteSize();
	int messageLen = message->ByteSize();
	requestScanLength =  2 + headerLen + 2 + messageLen;

	char* output = new char[requestScanLength];

	output[0] = ((headerLen >> 8) & 0xff);
	output[1] = (headerLen & 0xff);

	header.SerializePartialToArray(&output[2],headerLen);

	output[2 + headerLen] = ((messageLen >> 8) & 0xff);
	output[2 + headerLen + 1] = (messageLen & 0xff);

	message->SerializePartialToArray(&output[2 + headerLen + 2],messageLen);

	requestScan = output;
}

RoboclawProxy::~RoboclawProxy()
{
	delete [] requestScan;
	delete message;
}

int RoboclawProxy::FrontLeftSpeed()
{
	return frontLeftSpeed;
}

int RoboclawProxy::FrontRightSpeed()
{
	return frontRightSpeed;
}

int RoboclawProxy::RearLeftSpeed()
{
	return rearLeftSpeed;
}

int RoboclawProxy::RearRightSpeed()
{
	return rearRightSpeed;
}
	
double RoboclawProxy::GetSpeed()
{
	char *packetBytes;
	double speed;
	amber::roboclaw_proto::MotorsSpeed* currentSpeed;

	do
	{
		do
		{
			udp->Send(requestScan,requestScanLength);
			packetBytes = udp->Receive();
		}
		while(udp->n < 0);
	
	currentSpeed = motorsSpeedRequest(packetBytes);
	
	frontLeftSpeed =   currentSpeed->frontleftspeed(); 
	frontRightSpeed =  currentSpeed->frontrightspeed();
	rearLeftSpeed = currentSpeed->rearleftspeed();
	rearRightSpeed = currentSpeed->rearrightspeed();

	}
	while(!isSpeedOK(frontLeftSpeed,frontRightSpeed, rearLeftSpeed, rearRightSpeed));

	Vr = (double) (frontRightSpeed + rearRightSpeed) / 2000;
	Vl = (double) (frontLeftSpeed + rearLeftSpeed) / 2000;

	speed = ((frontLeftSpeed + frontRightSpeed + rearLeftSpeed + rearRightSpeed) / 4);
	speed = (speed / 1000);

	return speed;
}

bool RoboclawProxy::isSpeedOK(int frontLeftSpeed,int frontRightSpeed, int rearLeftSpeed,int rearRightSpeed)
{
	bool result = true;

	if((frontLeftSpeed < (-ROBOT_MAX_SPEED)) || (frontLeftSpeed > (ROBOT_MAX_SPEED)))
		result = false;

	if((frontRightSpeed < (-ROBOT_MAX_SPEED)) || (frontRightSpeed > (ROBOT_MAX_SPEED)))
		result = false;

	if((rearLeftSpeed < (-ROBOT_MAX_SPEED)) || (rearLeftSpeed > (ROBOT_MAX_SPEED)))
		result = false;

	if((rearRightSpeed < (-ROBOT_MAX_SPEED)) || (rearRightSpeed > (ROBOT_MAX_SPEED)))
		result = false;

	return result;
}

amber::roboclaw_proto::MotorsSpeed* RoboclawProxy::motorsSpeedRequest(char* packetBytes)
{
	int headerLen = (packetBytes[0] << 8) + (packetBytes[1] & 0xff);
	int messageLen = (packetBytes[2 + headerLen] << 8) + (packetBytes[2 + headerLen + 1] & 0xff);
	const char* shiftPacketBytes = &(packetBytes[2 + headerLen + 2]);

	message->ParseFromArray(shiftPacketBytes,messageLen);

	return message->MutableExtension(amber::roboclaw_proto::currentSpeed);
}

double RoboclawProxy::GetAngle(double time)
{
	double tmpRight = (double) (frontRightSpeed +  rearRightSpeed);
	double tmpLeftS = (double)(frontLeftSpeed + rearLeftSpeed);

return ((((tmpRight) / 2) - ((tmpLeftS) / 2)) * time) / wheelTrack;
}
