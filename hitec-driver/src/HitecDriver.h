#ifndef HITEC_DRIVER_H_
#define HITEC_DRIVER_H_

#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <log4cxx/logger.h>

#include "HitecCommon.h"

class HitecDriver {

public:
	HitecDriver(HitecConfiguration *configuration);
	virtual ~HitecDriver();

	void initializeDriver();
	void setAngle(unsigned int servo_address, int angle) throw (HitecSerialException);
	void setSpeed(unsigned int servo_address, int speed) throw (HitecSerialException);
	void setSameAngle(unsigned int *servo_addresses, int servo_count, int angle) throw (HitecSerialException);
	void setDifferentAngles(unsigned int *servo_addresses, int servo_count, int *angles) throw (HitecSerialException);

	boost::interprocess::interprocess_condition driverIsNotReady;
	boost::interprocess::interprocess_mutex serialPortMutex;

	bool driverReady;

private:

	static log4cxx::LoggerPtr _logger;

	int _fd;

	HitecConfiguration *_configuration;

};

#endif
