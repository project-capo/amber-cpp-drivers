/*
 * LocationController.h
 *
 *  Created on: 29-07-2014
 *      Author: szsz
 */

#ifndef LocationController_H_
#define LocationController_H_

#include <log4cxx/logger.h>
#include <boost/thread.hpp>
#include <boost/ref.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/thread/mutex.hpp>

#include "AmberScheduler.h"
#include "AmberPipes.h"
#include "drivermsg.pb.h"
#include "location.pb.h"
#include "Location.h"

boost::mutex location_mutex;

class LocationController: public MessageHandler {
public:
	LocationController(int pipeInFd, int pipeOutFd, const char *confFilename);
	virtual ~LocationController();

	void handleDataMsg(amber::DriverHdr *driverHdr, amber::DriverMsg *driverMsg);
	void handleClientDiedMsg(int clientID);
	void operator()();



private:
	AmberPipes *_amberPipes;

	bool _roboclawDisabled;
	bool _overheated;

	LocationConfiguration *_configuration;
	boost::thread* locationThread;
	//boost::thread *_errorMonitorThread;
	//	boost::thread *_temperatureMonitorThread;

	static log4cxx::LoggerPtr _logger;

	amber::DriverMsg *buildCurrentLocationMsg();
	void sendCurrentLocationMsg(int receiver, int ackNum);
	void handleCurrentLocationRequest(int sender, int synNum);

	void parseConfigurationFile(const char *filename);

	void locationMathod();

	Location *lok;
	void initLocation();
	void initLocation(char* mapPath);

	void handleCurrentUploadMapRequest(std::string sMap);

	void saveToFile(string sMap,string sPath);
};


#endif /* LocationController_H_ */
