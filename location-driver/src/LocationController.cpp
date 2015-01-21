/*
 * LocationController.cpp
 *
 *  Created on: 29-07-2014
 *      Author: szsz
 */

#include <log4cxx/propertyconfigurator.h>
#include <boost/program_options.hpp>
#include <string>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>


#include "LocationCommon.h"
#include "LocationController.h"

using namespace std;
using namespace boost;
using namespace boost::interprocess;
using namespace boost::program_options;
using namespace log4cxx;
using namespace amber;

LoggerPtr LocationController::_logger(Logger::getLogger("LocationController"));

LocationController::LocationController(int pipeInFd, int pipeOutFd, const char *confFilename)
{
	LOG4CXX_INFO(_logger, "LocationController");

	lok = NULL;

	parseConfigurationFile(confFilename);

	_amberPipes = new AmberPipes(this, pipeInFd, pipeOutFd);

	initLocation();
}

void LocationController::initLocation()
{
	LOG4CXX_INFO(_logger, "initLocation");


	//char* mapPath = "/home/szsz/git/Lokalizacja/Lokalizacja/Debug/labNowe3.roson";
	//char* mapPath = "/home//panda//Lokalizacja//Lokalizacja//labNowe3.roson";

	char* mapPath = new char[_configuration->mapPath.length()];
	strcpy(mapPath,_configuration->mapPath.c_str());

	initLocation(mapPath);

	delete mapPath;
}

void LocationController::initLocation(char* mapPath)
{
	LOG4CXX_INFO(_logger, "initLocation(char* mapPath)");

	try
	{

		unsigned int numberParticles = _configuration->NumberParticles;
		double epsilon = _configuration->epsilon;
		int generation = _configuration->generation;
		unsigned int ilosc_losowanych_nowych_czastek = _configuration->ilosc_losowanych_nowych_czastek;
		unsigned int przlieczenie_dla_pomiaru_skanera = _configuration->przlieczenie_dla_pomiaru_skanera;

		char* sIPPart = new char[_configuration->sIPPart.length()];
		strcpy(sIPPart,_configuration->sIPPart.c_str());


		lok = new Location(_logger,mapPath,numberParticles,epsilon,generation,ilosc_losowanych_nowych_czastek,przlieczenie_dla_pomiaru_skanera,sIPPart);

		locationThread = new boost::thread(boost::bind(&LocationController::locationMathod, this));
	}
	catch (std::exception& e) {
		LOG4CXX_ERROR(_logger, "Error in initLocation(char* mapPath): " << e.what());
		exit(1);
	}
}

LocationController::~LocationController() {
	LOG4CXX_INFO(_logger, "Stopping controller.");

	delete _amberPipes;
	delete lok;
}

void LocationController::handleDataMsg(amber::DriverHdr *driverHdr, amber::DriverMsg *driverMsg)
{
	try
	{
		if (_logger->isDebugEnabled()) {
			LOG4CXX_DEBUG(_logger, "Message came");
		}

		int clientId = driverHdr->clientids_size() > 0 ? driverHdr->clientids(0) : 0;

		// DataRequest
		if (driverMsg->HasExtension(location_proto::get_location))
		{
			if (!driverMsg->has_synnum()) {
				LOG4CXX_WARN(_logger, "Got CurrentLocationRequest, but syn num not set. Ignoring.");
				return;
			}

			if (driverMsg->GetExtension(location_proto::get_location))
			{
				handleCurrentLocationRequest(clientId, driverMsg->synnum());
			}
		}
		else if(driverMsg->HasExtension(location_proto::upload_map))
		{
			if (!driverMsg->has_synnum()) {
				LOG4CXX_WARN(_logger, "Got CurrentUploadMapRequest, but syn num not set. Ignoring.");
				return;
			}
			else
			{
				try
				{
					::std::string map = driverMsg->GetExtension(location_proto::upload_map);
					saveToFile(map,_configuration->uploadMapPath);

					LOG4CXX_INFO(_logger, "saveToFile(map,_configuration->uploadMapPath);");

					lok->StopLocation();

					LOG4CXX_INFO(_logger, "lok->StopLocation();");

					locationThread->join();

					LOG4CXX_INFO(_logger, "locationThread->join();");

					delete lok;

					LOG4CXX_INFO(_logger, "delete lok;");

					char* mapPath = new char[_configuration->uploadMapPath.length()];
					strcpy(mapPath,_configuration->uploadMapPath.c_str());

					LOG4CXX_INFO(_logger, "jestem tu");


					LOG4CXX_INFO(_logger,"likalizacjamapy" <<  mapPath);


					initLocation(mapPath);

					delete mapPath;

					LOG4CXX_INFO(_logger, "delete mapPath;");
				}
				catch (std::exception& e) {
					LOG4CXX_ERROR(_logger, "Error in parsing configuration file: " << e.what());
					exit(1);
				}
			}
		}
	}
	catch (std::exception& e)
	{
		LOG4CXX_ERROR(_logger, "handle DataMsg execption" << e.what());
		exit(1);
	}
}

void LocationController::handleClientDiedMsg(int clientID) {
	LOG4CXX_INFO(_logger, "Client " << clientID << " died");

}

void LocationController::operator()() 
{
	LOG4CXX_INFO(_logger, "operator()");
	try
	{
		_amberPipes->operator ()();
	}
	catch (std::exception& e)
	{
		LOG4CXX_ERROR(_logger, "operator()  execption:" << e.what());
	}
}

amber::DriverMsg *LocationController::buildCurrentLocationMsg()
{
	LOG4CXX_INFO(_logger, "build current location msg");

	amber::DriverMsg *message = new amber::DriverMsg();
	message->set_type(amber::DriverMsg_MsgType_DATA);

	location_proto::Location *currentLocation = message->MutableExtension(location_proto::currentLocation);

	location_mutex.lock();

	if(lok != NULL)
	{
		currentLocation->set_x(lok->Pos_X);
		currentLocation->set_y(lok->Pos_Y);
		currentLocation->set_p(lok->Prop);
		currentLocation->set_alfa(lok->Pos_Alfa);
		currentLocation->set_timestamp(lok->timeStamp);
	}
	else
	{
		currentLocation->set_x(0);
		currentLocation->set_y(0);
		currentLocation->set_p(0);
		currentLocation->set_alfa(0);
		currentLocation->set_timestamp(0);
	}

	location_mutex.unlock();

	return message;
}

void LocationController::sendCurrentLocationMsg(int receiver, int ackNum)
{
	if (_logger->isDebugEnabled()) {
		LOG4CXX_DEBUG(_logger, "Sending currentLocationRequest message");
	}

	amber::DriverMsg *currentLocationMsg = buildCurrentLocationMsg();
	currentLocationMsg->set_acknum(ackNum);
	amber::DriverHdr *header = new amber::DriverHdr();
	header->add_clientids(receiver);

	_amberPipes->writeMsgToPipe(header, currentLocationMsg);

	delete currentLocationMsg;
	delete header;
}

void LocationController::handleCurrentLocationRequest(int sender, int synNum)
{
	if (_logger->isDebugEnabled()) {
		LOG4CXX_DEBUG(_logger, "Handling currentLocationRequest message");
	}

	sendCurrentLocationMsg(sender, synNum);
}

void LocationController::handleCurrentUploadMapRequest(std::string sMap)
{
	LOG4CXX_INFO(_logger, "handleCurrentUploadMapRequest " << sMap);
	printf("%s\n",sMap.c_str());
}

void LocationController::parseConfigurationFile(const char *filename) {
	LOG4CXX_INFO(_logger, "Parsing configuration file: " << filename);

	_configuration = new LocationConfiguration();

	// unsigned int front_rc_address;
	//unsigned int rear_rc_address;


	options_description desc("Location options");
	desc.add_options()
            								("location.mapPath", value<string>(&_configuration->mapPath)->default_value("."))
            								("location.NumberParticles", value<unsigned int>(&_configuration->NumberParticles)->default_value(20))
            								("location.epsilon", value<double>(&_configuration->epsilon)->default_value(0.8))
            								("location.generation", value<int>(&_configuration->generation)->default_value(1))
            								("location.ilosc_losowanych_nowych_czastek", value<unsigned int>(&_configuration->ilosc_losowanych_nowych_czastek)->default_value(20))
            								("location.przlieczenie_dla_pomiaru_skanera", value<unsigned int>(&_configuration->przlieczenie_dla_pomiaru_skanera)->default_value(12))
            								("location.RandomWalkMaxDistance", value<double>(&_configuration->RandomWalkMaxDistance)->default_value(0.7))
            								("location.StandardDeviation", value<double>(&_configuration->StandardDeviation)->default_value(0.3))
            								("location.sIPPart", value<string>(&_configuration->sIPPart)->default_value("127.0.0.1"))
            								("location.uploadMapPath",  value<string>(&_configuration->uploadMapPath)->default_value("."));


	/*("roboclaw.motors_max_qpps", value<unsigned int>(&_configuration->motors_max_qpps)->default_value(13800))
            ("roboclaw.motors_p_const", value<unsigned int>(&_configuration->motors_p_const)->default_value(65536))
            ("roboclaw.motors_i_const", value<unsigned int>(&_configuration->motors_i_const)->default_value(32768))
            ("roboclaw.motors_d_const", value<unsigned int>(&_configuration->motors_d_const)->default_value(16384))
            ("roboclaw.pulses_per_revolution", value<unsigned int>(&_configuration->pulses_per_revolution)->default_value(1865))
            ("roboclaw.wheel_radius", value<unsigned int>(&_configuration->wheel_radius)->default_value(60))
            ("roboclaw.battery_monitor_interval", value<unsigned int>(&_configuration->battery_monitor_interval)->default_value(0))
            ("roboclaw.error_monitor_interval", value<unsigned int>(&_configuration->error_monitor_interval)->default_value(0))
            ("roboclaw.temperature_monitor_interval", value<unsigned int>(&_configuration->temperature_monitor_interval)->default_value(0))
            ("roboclaw.temperature_critical", value<__u16>(&_configuration->temperature_critical)->default_value(70))
            ("roboclaw.temperature_drop", value<__u16>(&_configuration->temperature_drop)->default_value(60))
            ("roboclaw.critical_read_repeats", value<unsigned int>(&_configuration->critical_read_repeats)->default_value(0));

	 */
	variables_map vm;

	try {
		store(parse_config_file<char>(filename, desc), vm);
		notify(vm);

		//_configuration->front_rc_address = (__u8) front_rc_address;
		//_configuration->rear_rc_address = (__u8) rear_rc_address;

	} catch (std::exception& e) {
		LOG4CXX_ERROR(_logger, "Error in parsing configuration file: " << e.what());
		exit(1);
	}
}

void LocationController::locationMathod()
{
	LOG4CXX_INFO(_logger, "Location Method -> RunLocation");

	lok->RunLocation();
}

void LocationController::saveToFile(string sMap,string sPath)
{
	ofstream myfile;
	myfile.open(sPath.c_str(),ios::trunc);
	myfile << sMap;
	myfile.close();
}

int main(int argc, char *argv[]) {

	if (argc < 3) {
		return 1;
	}

	const char *confFile =  argv[1];
	const char *logConfFile = argv[2];

	PropertyConfigurator::configure(logConfFile);

	// STDIN_FD = 0, STDOUT_FD = 1
	// pipe_in_fd = 0, pipe_out_fd = 1
	LoggerPtr logger(Logger::getLogger("main"));

	LOG4CXX_INFO(logger, "-------------");
	LOG4CXX_INFO(logger, "Creating controller, config_file: " << argv[1] << ", log_config_file: " << argv[2]);


	LocationController controller(0, 1, confFile);
	controller();



}
