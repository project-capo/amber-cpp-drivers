
#include "drivermsg.pb.h"
#include "roboclaw.pb.h"
#include "hokuyo.pb.h"

#include <stdio.h>
#include <log4cxx/propertyconfigurator.h>
using namespace log4cxx;

#define M_PI       3.14159265358979323846

class HokuyoProxy 
{
private:
	LoggerPtr _logger;

	static const int DEVICE_TYPE = 4;
	static const int DEVICE_ID = 0;

	UdpClient* udp;
    double *angles;
	int *distances;

	char* requestScan;
	int requestScanLength;
	amber::hokuyo_proto::Scan* scanRequest(char* packetBytes);
	amber::DriverMsg *message;
	
	amber::DriverHdr  buildHeader();
	amber::DriverMsg* buildMsg(int synNum);
	void buildSendMessage(amber::DriverHdr header, amber::DriverMsg* message);

	int synNum;

	unsigned int SkipScan;

public:

	HokuyoProxy(LoggerPtr logger,UdpClient* client_udp,unsigned int skipScan);
	~HokuyoProxy();
	
	int ScanLengthAll;
	int ScanLength;

	double Angle(int index);
	int Distance(int index);
	
    void GetScan();

    double* GetAngles(void);

    int* GetDistances(void);

    double ConvertToRadian(double degree);
};
