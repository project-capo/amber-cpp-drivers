/*
 * Location2.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: szsz
 */

#include "Location2.h"

Location2::Location2(LoggerPtr logger, Settings settings) {
	_logger = logger;
	_settings = &settings;
	LOG4CXX_INFO(_logger, "Location 2");

	this->Pos_X = 0;
	this->Pos_Y = 0;
	this->Prop = 0;
	this->Pos_Alfa = 0;
	this->timeStamp = 0;

	LOG4CXX_INFO(_logger, "Before: AmberClient UdpClient");
	AmberClientRoboClaw = new UdpClient(settings.RobotIP, 26233);
	AmberClientHokuyo = new UdpClient(settings.RobotIP, 26233);
	LOG4CXX_INFO(_logger, "After: AmberClient UdpClient");

	LOG4CXX_INFO(_logger, "Before: InitRoomsTable");
	InitRoomsTable(settings.PathMap);
	LOG4CXX_INFO(_logger, "After: InitRoomsTable");

	LOG4CXX_INFO(_logger, "Before: RoboclawProxy()");
	RoboClaw = new RoboclawProxy(_logger, AmberClientRoboClaw);
	LOG4CXX_INFO(_logger, "After: RoboclawProxy()");

	LOG4CXX_INFO(_logger, "Before: HokuyoProxy()");
	Hokuyo = new HokuyoProxy(_logger, AmberClientHokuyo, settings.SkipScan);
	LOG4CXX_INFO(_logger, "After: HokuyoProxy()");

	kht = new KernelBasedHoughTransform(settings.maxMeasuredDistance,
			settings.houghResolution);

	InitParticleTable(settings.NumberParticles);

	IsWorking = true;

#if DIAGNOSTIC_MODE == 1
	diagnosticVisualisation = new DiagnosticVisualisation(new UdpClient(settings.DiagnosticIPAddress, 1234),settings.NumberParticles,Particles,RoboClaw,Hokuyo);
#endif
}

Location2::~Location2() {
	LOG4CXX_INFO(_logger, "~Location");

	delete Particles;

	delete kht;

	delete Hokuyo;
	delete RoboClaw;

	delete AmberClientHokuyo;
	delete AmberClientRoboClaw;

#if DIAGNOSTIC_MODE == 1
	delete diagnosticVisualisation;
#endif
}

void Location2::InitRoomsTable(char *pathMap) {
	//tworzy pokoje
	//countRoomAndBox = parseJasonFile(pathMap,rooms);
}

void Location2::InitParticleTable(unsigned int numberParticles) {
	Particles = new Particle2[numberParticles]();

	//for (unsigned int i = 0; i < numberParticles; i++)
	//	Particles[i] = new Particle2();
}

void Location2::RunLocation()
{
	LOG4CXX_INFO(_logger, "RunLocation");



}
