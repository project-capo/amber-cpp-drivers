
#include "Location.h"

#define EXPR_SIZE   1024
#define BUFLEN      512
#define TRUE        1
#define SERVERLEN   1024

Location::Location(LoggerPtr logger)
{
	_logger = logger;
	LOG4CXX_INFO(_logger, "Location");

	char* robotIP = "192.168.2.100"; //getRobotIPAdress(); //nie moze byc localhost tylko adres robota

	clientAruco = new UdpServerLinux(53000);

	this->Pos_X = 0;
	this->Pos_Y = 0;
	this->Prop = 0;
	this->Pos_Alfa = 0;
	this->timeStamp = 0;
}

Location::~Location()
{
	LOG4CXX_INFO(_logger, "~Location");

	delete clientAruco;
}

void Location::RunLocation()
{
	LOG4CXX_INFO(_logger, "RunLocation");
	char* buffer;
	Aruco::ArucoLocation arLocation;

	while(work)
	{
		buffer = clientAruco->Receive();

		if(clientAruco->ReciveBufLen > -1)
		{
			arLocation.ParseFromArray(buffer,clientAruco->ReciveBufLen);

			LOG4CXX_DEBUG(_logger, "Recive Aruco Data");

			//przypisanie otrzymanej wartosci
			this->Pos_X = arLocation.x();
			this->Pos_Y = arLocation.y();
			this->Pos_Alfa = arLocation.alfa();
			this->Prop = 1.0;
			this->timeStamp = (long long int) (end.tv_sec * 1000.0 + end.tv_usec/1000.0);

			//cout << "Lok" <<  this->Pos_X << "   " << this->Pos_Y  << endl;
		}
	}
}

void Location::StopLocation()
{
	work = false;
}

char* Location::getRobotIPAdress()
{
	int fd;
	struct ifreq ifr;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	 /* I want to get an IPv4 IP address */
	 ifr.ifr_addr.sa_family = AF_INET;

	 /* I want IP address attached to "eth0" */
	 strncpy(ifr.ifr_name, "lo", IFNAMSIZ-1);

	 ioctl(fd, SIOCGIFADDR, &ifr);

	 close(fd);

	 return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
}

