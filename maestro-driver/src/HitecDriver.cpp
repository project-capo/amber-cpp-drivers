#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <linux/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <termios.h>

#include <log4cxx/logger.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/thread_time.hpp>

#include "HitecCommon.h"
#include "HitecDriver.h"
#include "HitecLib.h"

using namespace std;
using namespace boost;
using namespace boost::interprocess;
using namespace log4cxx;
using namespace boost::posix_time;
LoggerPtr HitecDriver::_logger (Logger::getLogger("Hitec.Driver"));

HitecDriver::HitecDriver(HitecConfiguration *configuration):
		driverReady(false), _configuration(configuration) {
}

HitecDriver::~HitecDriver() {
	LOG4CXX_INFO(_logger, "Stopping driver."); 
	ht_uart_close(_fd);
}

void HitecDriver::initializeDriver() {
	scoped_lock<interprocess_mutex> lock(serialPortMutex);

	_fd = ht_uart_open(_configuration->uart_port.c_str());
	if (_fd == -1) {
		LOG4CXX_FATAL(_logger, "Unable to open uart port: " << _configuration->uart_port);
		exit(1);
	}

	speed_t uart_speed;
	switch(_configuration->uart_speed) {

	case 2400:
		uart_speed = B2400;
		break;

	case 9600:
		uart_speed = B9600;
		break;

	case 19200:
		uart_speed = B19200;
		break;

	case 38400:
		uart_speed = B38400;
		break;

	default:
		LOG4CXX_FATAL(_logger, "Unknown uart speed: " << _configuration->uart_speed << ". Aborting.");
		exit(1);
	}

	LOG4CXX_INFO(_logger, "Initializing driver, port: " << _configuration->uart_port.c_str()
			<< ", baud: " << uart_speed);

	ht_uart_init(_fd, uart_speed);

	driverReady = true;
	driverIsNotReady.notify_all();
}

void HitecDriver::setAngle(unsigned int servo_address, int angle) throw (HitecSerialException) {
	scoped_lock<interprocess_mutex> lock(serialPortMutex);

	while (!driverReady) {
		driverIsNotReady.wait(lock);
	}

	if (_logger->isDebugEnabled()) {
		LOG4CXX_DEBUG(_logger, "ht_set_angle, sa: " << servo_address << ", a: " << angle);
	}

	if (ht_set_angle(_fd, servo_address, angle) < 0) {
		LOG4CXX_WARN(_logger, "ht_set_angle, sa: " << servo_address << ", a: " << angle << ": error");
	}
}

void HitecDriver::setSpeed(unsigned int servo_address, int speed) throw (HitecSerialException) {
	scoped_lock<interprocess_mutex> lock(serialPortMutex);

	while (!driverReady) {
		driverIsNotReady.wait(lock);
	}

	if (_logger->isDebugEnabled()) {
		LOG4CXX_DEBUG(_logger, "ht_set_speed, sa: " << servo_address << ", s: " << speed);
	}

	if (ht_set_speed(_fd, servo_address, speed) < 0) {
		LOG4CXX_WARN(_logger, "ht_set_speed, sa: " << servo_address << ", s: " << speed << ": error");
	}
}

void HitecDriver::setSameAngle(unsigned int *servo_addresses, int servo_count, int angle) throw (HitecSerialException) {
	scoped_lock<interprocess_mutex> lock(serialPortMutex);

	while (!driverReady) {
		driverIsNotReady.wait(lock);
	}

	if(servo_addresses == NULL) {
		LOG4CXX_WARN(_logger, "ht_set_same_angle, sa: NPE error");
		return;
	}

	if (_logger->isDebugEnabled()) {
		LOG4CXX_DEBUG(_logger, "ht_set_same_angle, sc: " << servo_count << ", a: " << angle);
		for(int i = 0; i < servo_count; i++) {
			LOG4CXX_DEBUG(_logger, "sa: " << *(servo_addresses + i));
		}
	}

	if (ht_set_same_angle(_fd, servo_addresses, servo_count, angle) < 0) {
		LOG4CXX_WARN(_logger, "ht_set_same_angle, sc: " << servo_count << ", a: " << angle << ": error");
		for(int i = 0; i < servo_count; i++) {
			LOG4CXX_WARN(_logger, "sa: " << *(servo_addresses + i));
		}
	}
}

void HitecDriver::setDifferentAngles(unsigned int *servo_addresses, int servo_count, int *angles) throw (HitecSerialException) {
	scoped_lock<interprocess_mutex> lock(serialPortMutex);

	while (!driverReady) {
		driverIsNotReady.wait(lock);
	}

	if(servo_addresses == NULL) {
		LOG4CXX_WARN(_logger, "ht_set_different_angles, sa: NPE error");
		return;
	}

	if(angles == NULL) {
		LOG4CXX_WARN(_logger, "ht_set_different_angles, a: NPE error");
		return;
	}

	if (_logger->isDebugEnabled()) {
		LOG4CXX_DEBUG(_logger, "ht_set_different_angles, sc: " << servo_count);
		for(int i = 0; i < servo_count; i++) {
			LOG4CXX_DEBUG(_logger, "sa: " << *(servo_addresses + i) << ", a: " << *(angles + i));
		}
	}

	if (ht_set_different_angles(_fd, servo_addresses, servo_count, angles) < 0) {
		LOG4CXX_WARN(_logger, "ht_set_different_angles, sc: " << servo_count);
		for(int i = 0; i < servo_count; i++) {
			LOG4CXX_WARN(_logger, "sa: " << *(servo_addresses + i) << ", a: " << *(angles + i));
		}
	}
}

