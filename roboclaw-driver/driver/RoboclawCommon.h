/*
 * RoboclawCommon.h
 *
 *  Created on: 02-12-2012
 *      Author: michal
 */

#ifndef ROBOCLAWCOMMON_H_
#define ROBOCLAWCOMMON_H_

#include <linux/types.h>
#include <string>

struct CurrentSpeedStruct {
	int m1_speed;
	bool m1_direction;
	int m2_speed;
	bool m2_direction;

	CurrentSpeedStruct(): m1_speed(0), m2_speed(0) {}
};

struct MotorsCommandStruct {

	int frontLeftSpeed;
	int frontRightSpeed;
	int rearLeftSpeed;
	int rearRightSpeed;

};

struct RoboclawConfiguration {

	std::string uart_port;
	unsigned int uart_speed;

	__u8 front_rc_address;
	__u8 rear_rc_address;

	__u32 motors_max_qpps;
	__u32 motors_p_const;
	__u32 motors_i_const;
	__u32 motors_d_const;

	__u32 pulses_per_revolution;
	__u32 wheel_radius;
};


#endif /* ROBOCLAWCOMMON_H_ */
