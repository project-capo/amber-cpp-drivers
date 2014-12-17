/*
 * LocationCommon.h
 *
 *  Created on: 11-10-2014
 *      Author: szsz
 */

#ifndef LOCATIONCOMMON_H_
#define LOCATIONCOMMON_H_

#include <exception>
#include <linux/types.h>
#include <string>

struct LocationConfiguration {

	std::string mapPath;
	unsigned int NumberParticles;
	double epsilon;
	int generation;
	unsigned int ilosc_losowanych_nowych_czastek;
	unsigned int przlieczenie_dla_pomiaru_skanera ;
	double RandomWalkMaxDistance;
	double StandardDeviation;
	std::string sIPPart;
	std::string uploadMapPath;

/*	std::string uart_port;
	unsigned int uart_speed;

	std::string reset_gpio_path;
	__u32 reset_delay;

	std::string led1_gpio_path;
	std::string led2_gpio_path;

	__u8 front_rc_address;
	__u8 rear_rc_address;

	__u32 motors_max_qpps;
	__u32 motors_p_const;
	__u32 motors_i_const;
	__u32 motors_d_const;

	__u32 pulses_per_revolution;
	__u32 wheel_radius;

	__u32 battery_monitor_interval;
	__u32 error_monitor_interval;
	__u32 temperature_monitor_interval;

	__u16 temperature_critical;
	__u16 temperature_drop;

	__u32 critical_read_repeats;

	__u32 stop_idle_timeout;
	__u32 reset_idle_timeout;
*/
};

/*
class RoboclawSerialException: public std::exception {

};*/


#endif /* LOCATIONCOMMON_H_ */
