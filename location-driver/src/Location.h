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
#include "HokuyoProxy.h"
#include "RoboclawProxy.h"
#include "LocationProxy.h"

#include "BoundingBox.h"
#include "Particle.h"

#include <math.h>

#include <log4cxx/propertyconfigurator.h>
#include <boost/thread/mutex.hpp>
using namespace log4cxx;


#define M_PI 3.14159265358979323846

#define DIAGNOSTIC 1

using namespace std;

class Location
{
private:
	char* amberUdp;
	UdpClient *clinetAmber;

	BoundingBox* bBox;
	Room* rooms;
	int countBox;
	int countRoomAndBox;

	Particle* tablicaCzastek;
	unsigned int NumberParticles;
	int iloscCzastekDoUsuniacia;

	RoboclawProxy* roboClaw;
	double speedRoboClaw;
	double angleRoboClaw;

	HokuyoProxy* skaner;

	struct timeval start,end;

	double deletaTime;
	Room* currentRoom;

	double EPSILON;
	int GENERATION;
	unsigned int ILOSC_LOSOWANYCH_NOWYCH_CZASTEK;

	bool work;

	void RozmiescCzastki(BoundingBox* bBox,unsigned int BoundingBoxCount,Particle* tablicaCzastek,unsigned int ParticleCount);
	void InitTablicaCzastekLosowo(Particle *tablica,BoundingBox* bBox,int countBox);
	Room* GetRoom(Room* bBox,int length, double X,double Y);

	const char* SendParticle(string *diagnostic,Particle *tab);

    static int compareMyType (const void * a, const void * b);

	void UsunWylosujNoweCzastki2(Particle* tablicaCzastek,unsigned int iloscCzastekDoUsuniecia);
	inline int wylosujBB(int fMax);

	char* getRobotIPAdress();

	void UsunWylosujNoweCzastki3(Particle* tablicaCzastek,unsigned int length,unsigned int iloscCzastekDoUsuniecia,BoundingBox* bBox,unsigned int BoundingBoxCount);
	void UsunWylosujNoweCzastki4(Particle* tablicaCzastek,unsigned int length,unsigned int iloscCzastekDoUsuniecia);
	void UsunWylosujNoweCzastki5(Particle* tablicaCzastek,unsigned int length);
	void UsunWylosujNoweCzastki6(Particle* tablicaCzastek,unsigned int length,unsigned int iloscCzastekDoUsuniecia);
	void UsunWylosujNoweCzastki7(Particle* tablicaCzastek,unsigned int length,unsigned int iloscCzastekDoUsuniecia,double wheelTrack, double VL, double Vr,double dt);

	void UsunWylosujNoweCzastki8(Particle* tablicaCzastek,unsigned int length,unsigned int iloscCzastekDoUsuniecia);
	void UsunWylosujNoweCzastki68(Particle* tablicaCzastek,unsigned int length,unsigned int iloscCzastekDoUsuniecia);

	void UsunWylosujNoweCzastki68a(Particle* tablicaCzastek,unsigned int length,unsigned int iloscCzastekDoUsuniecia);

	void UsunWylosujNoweCzastki9(Particle* ttablicaCzastek,unsigned int length,unsigned int iloscCzastekDoUsuniecia,Room* bbBox,int lengthBBbox);

	void UsunWylosujNoweCzastki10(Particle* ttablicaCzastek,unsigned int length,unsigned int iloscCzastekDoUsuniecia,double Vr,double Vl,double dt,double wheelTrack);

	/////// Diagnostic ////////////////


	int GetSelectedRooms(Room* & bbBox,int length,string selectedRooms[],int selectedRoomsCount);
#if DIAGNOSTIC == 1
	char* IPPart;
	UdpClient *clientParticle;

	string diagnostic;
	int size;
	const char* wys;
#endif
	LoggerPtr _logger;

public:
	Location(LoggerPtr _logger,char* mapPath,unsigned int numberParticles,double epsilon,int generation,unsigned int ilosc_losowanych_nowych_czastek,unsigned int skipScan,char* sIPPart);
	~Location();
	void RunLocation();
	void StopLocation();

	double Pos_X;
	double Pos_Y;
	double Prop;
	double Pos_Alfa;
	long long int timeStamp;

};


