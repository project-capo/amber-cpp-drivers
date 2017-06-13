#ifndef LOCATION_H_
#define LOCATION_H_

#include <stdio.h>
#include <ctime>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>

#include "UdpClientLinux.h"
#include "UdpServerLinux.h"
#include "ArucoLocation.pb.h"

#include <math.h>

#include <log4cxx/propertyconfigurator.h>
#include <boost/thread/mutex.hpp>

using namespace log4cxx;

#define M_PI 3.14159265358979323846

using namespace std;

class Location
{
private:
	UdpServerLinux *clientAruco;

	LoggerPtr _logger;
	bool work;
	struct timeval start,end;
	char* getRobotIPAdress();

public:
	Location(LoggerPtr _logger);
	~Location();
	void RunLocation();
	void StopLocation();

	double Pos_X;
	double Pos_Y;
	double Prop;
	double Pos_Alfa;
	long long int timeStamp;

};

#endif /* LOCATION_H_ */

