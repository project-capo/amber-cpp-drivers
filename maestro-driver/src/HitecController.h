/*
 * StargazerController.h
 *
 *
 *  Created on: 30-11-2012
 *      Author: michal
 */

#ifndef HitecCONTROLLER_H_
#define HitecCONTROLLER_H_

#include <log4cxx/logger.h>
#include <boost/thread.hpp>
#include <boost/ref.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "AmberScheduler.h"
#include "AmberPipes.h"
#include "HitecDriver.h"
#include "drivermsg.pb.h"
#include "hitec.pb.h"
#include "HitecLib.h"

class HitecController: public MessageHandler {
public:
	HitecController(int pipeInFd, int pipeOutFd, const char *confFilename);
	virtual ~HitecController();

	void handleDataMsg(amber::DriverHdr *driverHdr, amber::DriverMsg *driverMsg);

	void runAmberPipesProcess();

private:
	HitecDriver *_hitecDriver;
	AmberPipes *_amberPipes;

	bool _hitecDisabled;
	bool _overheated;

	HitecConfiguration *_configuration;

	static log4cxx::LoggerPtr _logger;

	void handleSetAngleCommand(amber::hitec_proto::SetAngle *setAngleCommand);
	void handleSetSameAngleCommand(amber::hitec_proto::SetSameAngle *setSameAngleCommand);
	void handleSetDifferentAngleCommand(amber::hitec_proto::SetDifferentAngles *setDifferentAnglesCommand);
	void handleSetSpeedCommand(amber::hitec_proto::SetSpeed *setSpeedCommand);
	void parseConfigurationFile(const char *filename);

	void handleClientDiedMsg(int clientID);
};


#endif /* HitecCONTROLLER_H_ */
