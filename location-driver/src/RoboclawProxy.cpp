#include "UdpClientLinux.h"
#include "RoboclawProxy.h"

RoboclawProxy::RoboclawProxy(LoggerPtr logger,UdpClient *client_udp)
{
	_logger = logger;

	LOG4CXX_INFO(_logger, "RoboclawProxy");

	udp = client_udp;

	synNum = 100;
	wheelTrack = 0.420;//wheelTrack = 0.280;

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

amber::DriverMsg* RoboclawProxy::buildMsg(int isynNum)
{
	amber::DriverMsg* msg = new amber::DriverMsg();
	msg->set_type(amber::DriverMsg::DATA);

	msg->SetExtension(amber::roboclaw_proto::currentSpeedRequest,true);
	msg->set_synnum(isynNum);

	return msg;
}

void RoboclawProxy::buildSendMessage(amber::DriverHdr header, amber::DriverMsg* msg)
{
	int headerLen = header.ByteSize();
	int messageLen = msg->ByteSize();
	requestScanLength =  2 + headerLen + 2 + messageLen;

	char* output = new char[requestScanLength];

	output[0] = (char) ((headerLen >> 8) & 0xff);
	output[1] = (char) (headerLen & 0xff);

	header.SerializePartialToArray(&output[2],headerLen);

	output[2 + headerLen] = (char) ((messageLen >> 8) & 0xff);
	output[2 + headerLen + 1] = (char) (messageLen & 0xff);

	msg->SerializePartialToArray(&output[2 + headerLen + 2],messageLen);

	requestScan = output;
}

RoboclawProxy::~RoboclawProxy()
{
	LOG4CXX_INFO(_logger, "~RoboclawProxy");

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
	LOG4CXX_INFO(_logger, "GetSpeed");

	bool isException;
	char *packetBytes;
	double speed;
	amber::roboclaw_proto::MotorsSpeed* currentSpeed;

	do
	{
		try
		{
			isException = false;

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
		}
		catch(std::exception& e)
		{
			LOG4CXX_INFO(_logger, "GetScan Error" << e.what());
			isException = true;
		}
		catch(...)
		{
			LOG4CXX_INFO(_logger, "GetScan Error Unknown");
			isException = true;
		}
	}
	while(isException);

	return speed;
}

bool RoboclawProxy::isSpeedOK(int ifrontLeftSpeed,int ifrontRightSpeed, int irearLeftSpeed,int irearRightSpeed)
{
	bool result = true;

	if((ifrontLeftSpeed < (-ROBOT_MAX_SPEED)) || (ifrontLeftSpeed > (ROBOT_MAX_SPEED)))
		result = false;

	if((ifrontRightSpeed < (-ROBOT_MAX_SPEED)) || (ifrontRightSpeed > (ROBOT_MAX_SPEED)))
		result = false;

	if((irearLeftSpeed < (-ROBOT_MAX_SPEED)) || (irearLeftSpeed > (ROBOT_MAX_SPEED)))
		result = false;

	if((irearRightSpeed < (-ROBOT_MAX_SPEED)) || (irearRightSpeed > (ROBOT_MAX_SPEED)))
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
