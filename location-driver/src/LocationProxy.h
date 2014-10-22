#include "drivermsg.pb.h"
#include "location.pb.h"

class LocationProxy
{
private:

	static const int DEVICE_TYPE = 6;
	static const int DEVICE_ID = 0;

	int synNum;

	UdpClient* udp;

	double pos_X;
	double pos_Y;

	char* requestLocation;
	int requestLocationLength;

	amber::DriverMsg *message;
	amber::location_proto::Location* locationRequest(char* packetBytes);

	amber::DriverHdr  buildHeader();
	amber::DriverMsg* buildMsg(int synNum);
	void buildSendMessage(amber::DriverHdr header, amber::DriverMsg* message);

public:

	LocationProxy(UdpClient* client_udp);
	~LocationProxy();

	double X();
	double Y();

    void GetLocation();

};
