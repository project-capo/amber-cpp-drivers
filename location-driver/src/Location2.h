/*
 * Location2.h
 *
 *  Created on: Nov 7, 2016
 *      Author: szsz
 */

#ifndef LOCATION2_H_
#define LOCATION2_H_

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
#include <math.h>

#include <log4cxx/propertyconfigurator.h>
#include <boost/thread/mutex.hpp>

#include "Settings.h"
#include "Common.h"

#include "UdpClientLinux.h"
#include "HokuyoProxy.h"
#include "RoboclawProxy.h"
#include "KernelBasedHoughTransform.h"
#include "Particle2.h"
#include "DiagnosticVisualisation.h"

using namespace log4cxx;

class Location2 {
private:
	LoggerPtr _logger;

#if DIAGNOSTIC_MODE == 1
	DiagnosticVisualisation *diagnosticVisualisation;
#endif

private:
	Settings *_settings;
	bool IsWorking;

	UdpClient *AmberClientRoboClaw;
	UdpClient *AmberClientHokuyo;

	RoboclawProxy* RoboClaw;
	HokuyoProxy* Hokuyo;

	KernelBasedHoughTransform * kht;

	Particle2 *Particles;

	unsigned int RoomsLength;
	Room* Rooms;

private:
	void InitRoomsTable(char *pathMap);
	void InitParticleTable(unsigned int numberParticles);

	Room* GetRoom(double X,double Y);

public:
	double Pos_X;
		double Pos_Y;
		double Prop;
		double Pos_Alfa;
		long long int timeStamp;
public:
	Location2(LoggerPtr logger, Settings *settings);
	virtual ~Location2();

	void RunLocation();


};

#endif /* LOCATION2_H_ */
