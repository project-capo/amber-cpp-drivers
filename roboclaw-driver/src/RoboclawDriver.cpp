/*
 * StargazerDriver.cpp
 *
 *  Created on: 30-11-2012
 *      Author: michal
 */
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

#include "RoboclawCommon.h"
#include "RoboclawDriver.h"
#include "RoboclawLib.h"

using namespace std;
using namespace boost;
using namespace boost::interprocess;
using namespace log4cxx;
using namespace boost::posix_time;
LoggerPtr RoboclawDriver::_logger (Logger::getLogger("Roboclaw.Driver"));

RoboclawDriver::RoboclawDriver(RoboclawConfiguration *configuration):
		driverReady(false), _configuration(configuration) {

}

RoboclawDriver::~RoboclawDriver() {
	LOG4CXX_INFO(_logger, "Stopping driver."); 
	rc_uart_close(_fd);
	rc_gpio_close(_gpioFd);

}

void RoboclawDriver::initializeDriver() {
	scoped_lock<interprocess_mutex> lock(serialPortMutex);

	_fd = rc_uart_open(_configuration->uart_port.c_str());
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
	rc_uart_init(_fd, uart_speed);

	// Setting motors constants
	if (rc_set_pid_consts_m1(_fd, _configuration->front_rc_address, _configuration->motors_d_const,
			_configuration->motors_p_const, _configuration->motors_i_const, _configuration->motors_max_qpps) < 0) {
		LOG4CXX_WARN(_logger, "rc_set_pid_consts_m1, " << (int)_configuration->front_rc_address << ": error");
	}
	if (rc_set_pid_consts_m2(_fd, _configuration->front_rc_address, _configuration->motors_d_const,
				_configuration->motors_p_const, _configuration->motors_i_const, _configuration->motors_max_qpps) < 0) {
		LOG4CXX_WARN(_logger, "rc_set_pid_consts_m2, " << (int)_configuration->front_rc_address << ": error");
	}

	if (rc_set_pid_consts_m1(_fd, _configuration->rear_rc_address, _configuration->motors_d_const,
				_configuration->motors_p_const, _configuration->motors_i_const, _configuration->motors_max_qpps) < 0) {
		LOG4CXX_WARN(_logger, "rc_set_pid_consts_m1, " << (int)_configuration->rear_rc_address << ": error");
	}

	if (rc_set_pid_consts_m2(_fd, _configuration->rear_rc_address, _configuration->motors_d_const,
				_configuration->motors_p_const, _configuration->motors_i_const, _configuration->motors_max_qpps) < 0) {
		LOG4CXX_WARN(_logger, "rc_set_pid_consts_m2, " << (int)_configuration->rear_rc_address << ": error");
	}

	_gpioFd = rc_gpio_open(_configuration->reset_gpio_path.c_str());
	if (_gpioFd == -1) {
		LOG4CXX_FATAL(_logger, "Unable to open reset gpio: " << _configuration->reset_gpio_path);
		exit(1);
	}

	driverReady = true;
	driverIsNotReady.notify_all();
}

void RoboclawDriver::readCurrentSpeed(MotorsSpeedStruct *mss) {
	scoped_lock<interprocess_mutex> lock(serialPortMutex);

	while (!driverReady) {
		driverIsNotReady.wait(lock);
	}

	unsigned int flQpps, frQpps, rlQpps, rrQpps;
	unsigned char flDir, frDir, rlDir, rrDir;

	if (rc_read_speed_m1(_fd, _configuration->front_rc_address, &frQpps, &frDir) < 0) {
		LOG4CXX_WARN(_logger, "rc_read_speed_m1, " << (int)_configuration->front_rc_address << ": error");
	}

	if (rc_read_speed_m2(_fd, _configuration->front_rc_address, &flQpps, &flDir) < 0) {
		LOG4CXX_WARN(_logger, "rc_read_speed_m2, " << (int)_configuration->front_rc_address << ": error");
	}

	if (rc_read_speed_m1(_fd, _configuration->rear_rc_address, &rrQpps, &rrDir) < 0) {
		LOG4CXX_WARN(_logger, "rc_read_speed_m1, " << (int)_configuration->rear_rc_address << ": error");
	}

	if (rc_read_speed_m2(_fd, _configuration->rear_rc_address, &rlQpps, &rlDir) < 0) {
		LOG4CXX_WARN(_logger, "rc_read_speed_m2, " << (int)_configuration->rear_rc_address << ": error");
	}

	mss->frontLeftSpeed = flDir == 0 ? (int)flQpps : -(int)flQpps;
	mss->frontRightSpeed = frDir == 0 ? (int)frQpps : -(int)frQpps;
	mss->rearLeftSpeed = rlDir == 0 ? (int)rlQpps : -(int)rlQpps;
	mss->rearRightSpeed = rrDir == 0 ? (int)rrQpps : -(int)rrQpps;

	LOG4CXX_DEBUG(_logger, "current_speed, fl: " << mss->frontLeftSpeed << ", fr: " << mss->frontRightSpeed << ", rl: " << mss->rearLeftSpeed << ", rr: " << mss->rearRightSpeed);
	return;
}

void RoboclawDriver::stopMotors() {
	scoped_lock<interprocess_mutex> lock(serialPortMutex);

	while (!driverReady) {
		driverIsNotReady.wait(lock);
	}

	LOG4CXX_INFO(_logger, "Stopping motors.");
	rc_drive_forward(_fd, _configuration->front_rc_address, 0);
	rc_drive_forward(_fd, _configuration->rear_rc_address, 0);

}

void RoboclawDriver::sendMotorsEncoderCommand(MotorsSpeedStruct *mss) {
	scoped_lock<interprocess_mutex> lock(serialPortMutex);

	while (!driverReady) {
		driverIsNotReady.wait(lock);
	}

	if (mss != NULL) {
		LOG4CXX_DEBUG(_logger, "rc_drive_speed, fl: " << mss->frontLeftSpeed << ", fr: " << mss->frontRightSpeed << ", rl: " << mss->rearLeftSpeed << ", rr: " << mss->rearRightSpeed);

		if (rc_drive_speed(_fd, _configuration->front_rc_address, mss->frontRightSpeed, mss->frontLeftSpeed) < 0) {
			LOG4CXX_WARN(_logger, "rc_read_drive_speed, " << (int)_configuration->front_rc_address << ": error");
		}

		if (rc_drive_speed(_fd, _configuration->rear_rc_address, mss->rearRightSpeed, mss->rearLeftSpeed) < 0) {
			LOG4CXX_WARN(_logger, "rc_read_drive_speed, " << (int)_configuration->rear_rc_address << ": error");	
		}
	}
}

void RoboclawDriver::readMainBatteryVoltage(__u16 *voltage) {
	scoped_lock<interprocess_mutex> lock(serialPortMutex);

	while (!driverReady) {
		driverIsNotReady.wait(lock);
	}

	if (voltage != NULL) {
		if (rc_read_main_battery_voltage_level(_fd, _configuration->front_rc_address, voltage) < 0) {
			LOG4CXX_WARN(_logger, "rc_read_main_battery_voltage_level: error");
		}
	} 

}

void RoboclawDriver::readErrorStatus(__u8 *frontErrorStatus, __u8 *rearErrorStatus) {
	scoped_lock<interprocess_mutex> lock(serialPortMutex);

	while (!driverReady) {
		driverIsNotReady.wait(lock);
	}

	if (frontErrorStatus != NULL) {
		if (rc_read_error_status(_fd, _configuration->front_rc_address, frontErrorStatus) < 0) {
			LOG4CXX_WARN(_logger, "rc_read_error_status, " << (int)_configuration->front_rc_address << ": error");
		}
	}

	if (rearErrorStatus != NULL) {
		if (rc_read_error_status(_fd, _configuration->rear_rc_address, rearErrorStatus) < 0) {
			LOG4CXX_WARN(_logger, "rc_read_error_status, " << (int)_configuration->rear_rc_address << ": error");
		}
	}

}

void RoboclawDriver::readTemperature(__u16 *frontTemperature, __u16 *rearTemperature) {
	scoped_lock<interprocess_mutex> lock(serialPortMutex);

	while (!driverReady) {
		driverIsNotReady.wait(lock);
	}

	if (frontTemperature != NULL) {
		if (rc_read_temperature(_fd, _configuration->front_rc_address, frontTemperature) < 0) {
			LOG4CXX_WARN(_logger, "rc_read_temperature, " << (int)_configuration->front_rc_address << ": error");
		}
	}

	if (rearTemperature != NULL) {
		if (rc_read_temperature(_fd, _configuration->rear_rc_address, rearTemperature) < 0) {
			LOG4CXX_WARN(_logger, "rc_read_temperature, " << (int)_configuration->rear_rc_address << ": error");
		}
	}
}

void RoboclawDriver::reset() {
	scoped_lock<interprocess_mutex> lock(serialPortMutex);

	while (!driverReady) {
		driverIsNotReady.wait(lock);
	}

	if (rc_reset(_gpioFd) < 0) {
		LOG4CXX_WARN(_logger, "rc_reset: error");
	}
}