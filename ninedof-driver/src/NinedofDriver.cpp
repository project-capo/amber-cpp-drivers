/*
 * NinedofDriver.cpp
 *
 *  Created on: 29-10-2012
 *      Author: michal
 */

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <linux/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <log4cxx/logger.h>

#include "NinedofCommon.h"
#include "NinedofDriver.h"
#include "I2c.h"

using namespace std;
using namespace boost;
using namespace boost::interprocess;
using namespace log4cxx;

LoggerPtr NinedofDriver::_logger (Logger::getLogger("Ninedof.Driver"));


NinedofDriver::NinedofDriver(NinedofConfiguration *configuration):
	driverReady(false), dataReady(false), needToGetData(false), _configuration(configuration) {
}

NinedofDriver::~NinedofDriver() {

}

NinedofDataStruct *NinedofDriver::getDataStruct() {
	return &_dataStruct;
}

void NinedofDriver::operator()() {
	LOG4CXX_INFO(_logger, "Driver thread started.");

	// initialize driver
	{
		scoped_lock<interprocess_mutex> lock(driverReadyMutex);

		initializeDriver();

		LOG4CXX_INFO(_logger, "Driver process ready");
		driverReady = true;
		driverIsNotReady.notify_all();
	}


	driverLoop();
}

void NinedofDriver::lockUntilDriverReady() {
	scoped_lock<interprocess_mutex> lock(driverReadyMutex);
	driverIsNotReady.wait(lock);
}


void NinedofDriver::initializeDriver() {

#ifdef MOCK
	LOG4CXX_INFO(_logger, "Initializing mock driver.");
#else

	_fd = i2c_open(_configuration->i2c_port.c_str());
	if (_fd == -1) {
		LOG4CXX_FATAL(_logger, "Unable to open i2c bus, port: " << _configuration->i2c_port);
		exit(1);
	}

	LOG4CXX_INFO(_logger, "Opened I2C bus.");

	__u8 tmp;

	/*
	* Accel configuration
	*/

	/* CTRL_REG1_A: low power disabled, 400Hz update rate, all axes enabled */
	tmp = 0x97;
	if(i2c_write(_fd, ACCEL_ADDRESS, ACCEL_CTRL_REG1_A, 1, &tmp) != 1) {
		LOG4CXX_FATAL(_logger, "Unable to write CTRL_REG1A");
		exit(1);
	}
	
	/* CTRL_REG4_A: scale, high res update mode */
	tmp = 0x18;
	if(i2c_write(_fd, ACCEL_ADDRESS, ACCEL_CTRL_REG4_A, 1, &tmp) != 1) {
		LOG4CXX_FATAL(_logger, "Unable to write CTRL_REG4A");
		exit(1);
	}

	/*
	* Compass configuration
	*/

	/* CRA_REG_M: temp sensor on, update rate set to 220Hz */
	tmp = 0x9C;
	if(i2c_write(_fd, MAGNET_ADDRESS, MAGNET_CRA_REG_M, 1, &tmp) != 1) {
		LOG4CXX_FATAL(_logger, "Unable to write CRA_REG_M");
		exit(1);
	}

	/* CRB_REG_M: gain setting */
	tmp = 0xE0;
	if(i2c_write(_fd, MAGNET_ADDRESS, MAGNET_CRB_REG_M, 1, &tmp) != 1) {
		LOG4CXX_FATAL(_logger, "Unable to write CRB_REG_M");
		exit(1);
	}

	/* MR_REG_M continous-conversion mode */
	tmp = 0x00;
	if(i2c_write(_fd, MAGNET_ADDRESS, MAGNET_MR_REG_M, 1, &tmp) != 1) {
		LOG4CXX_FATAL(_logger, "Unable to write MR_REG_M");
		exit(1);
	}

	/*
	* Gyro configuration
	*/

	/* CTRL_REG1 ODR, bandwidth, power down off, all axes enabled  */
	tmp = 0xEF;
	if(i2c_write(_fd, GYRO_ADDRESS, GYRO_CTRL_REG1, 1, &tmp) != 1) {
		LOG4CXX_FATAL(_logger, "Unable to write CTRL_REG1_A");
		exit(1);
	}

	/* CTRL_REG4 ODR, full scale  */
	tmp = 0x10;
	if(i2c_write(_fd, GYRO_ADDRESS, GYRO_CTRL_REG4, 1, &tmp) != 1) {
		LOG4CXX_FATAL(_logger, "Unable to write CTRL_REG1_A");
		exit(1);
	}

#endif
}

void NinedofDriver::driverLoop() {

#ifdef MOCK
	srand(time(NULL));
#else
	__u8 accel_axes[6];
	__u8 magnet_axes[6];
	__u8 gyro_axes[6];
#endif

	while (1) {
		scoped_lock<interprocess_mutex> lock(dataMutex);

		while (!needToGetData) {
			noNeedToGetData.wait(lock);
		}

#ifdef MOCK

		if (_logger->isDebugEnabled()) {
			LOG4CXX_DEBUG(_logger, "Randomizing new data, accel x_axis: " << _dataStruct.accel.x_axis);
		}

		_dataStruct.accel.x_axis = (rand() % 2000) - 1000;
		_dataStruct.accel.y_axis = (rand() % 2000) - 1000;
		_dataStruct.accel.z_axis = (rand() % 2000) - 1000;

		_dataStruct.gyro.x_axis = (rand() % 2000) - 1000;
		_dataStruct.gyro.y_axis = (rand() % 2000) - 1000;
		_dataStruct.gyro.z_axis = (rand() % 2000) - 1000;

		_dataStruct.magnet.x_axis = (rand() % 2000) - 1000;
		_dataStruct.magnet.y_axis = (rand() % 2000) - 1000;
		_dataStruct.magnet.z_axis = (rand() % 2000) - 1000;

#else

		if (_logger->isDebugEnabled()) {
			LOG4CXX_DEBUG(_logger, "Reading data from 9dof sensor");
		}

		if(i2c_read(_fd, ACCEL_ADDRESS, ACCEL_AXES_REG, 6, accel_axes) != 6) {
			LOG4CXX_WARN(_logger, "Unable to read from accel device");
		} else {
			_dataStruct.accel.x_axis = (__s16)(accel_axes[1]<<8 | accel_axes[0]);
			_dataStruct.accel.y_axis = (__s16)(accel_axes[3]<<8 | accel_axes[2]);
			_dataStruct.accel.z_axis = (__s16)(accel_axes[5]<<8 | accel_axes[4]);
		}

		if(i2c_read(_fd, GYRO_ADDRESS, GYRO_AXES_REG, 6, gyro_axes) != 6) {
			LOG4CXX_WARN(_logger, "Unable to read from gyro device");
		} else {
			_dataStruct.gyro.x_axis = (__s16)(gyro_axes[1]<<8 | gyro_axes[0]);
			_dataStruct.gyro.y_axis = (__s16)(gyro_axes[3]<<8 | gyro_axes[2]);
			_dataStruct.gyro.z_axis = (__s16)(gyro_axes[5]<<8 | gyro_axes[4]);
		}


		if(i2c_read(_fd, MAGNET_ADDRESS, MAGNET_AXES_REG, 6, magnet_axes) != 6) {
			LOG4CXX_WARN(_logger, "Unable to read from magnet device");
		} else {
			_dataStruct.magnet.x_axis = (__s16)(magnet_axes[1]<<8 | magnet_axes[0]);
			_dataStruct.magnet.y_axis = (__s16)(magnet_axes[3]<<8 | magnet_axes[2]);
			_dataStruct.magnet.z_axis = (__s16)(magnet_axes[5]<<8 | magnet_axes[4]);
		}

#endif


		// data ready
		needToGetData = false;
		dataNotReady.notify_all();
	}
}
