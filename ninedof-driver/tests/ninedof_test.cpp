#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/types.h>
#include <unistd.h>
#include "I2c.h"

#include <sys/time.h>
#include <time.h>

/* accelerometr and compass adress */
#define ACCEL_ADDRESS 0x19 
#define ACCEL_CTRL_REG1_A 0x20 
#define ACCEL_CTRL_REG4_A 0x23 
#define ACCEL_AXES_REG 0x28

#define MAGNET_ADDRESS 0x1E
#define MAGNET_CRA_REG_M 0x00 
#define MAGNET_CRB_REG_M 0x01 
#define MAGNET_MR_REG_M 0x02 
#define MAGNET_AXES_REG 0x03

#define GYRO_ADDRESS 0x6b
#define GYRO_CTRL_REG1 0x20
#define GYRO_CTRL_REG4 0x23
#define GYRO_AXES_REG 0x28


int file;

int main() {

	file = i2c_open("/dev/i2c-4");
	if (file == -1) {
		printf("Unable to open i2c bus.\n");
		exit(1);
	}

     	printf("Opened i2c bus\n");
	__u8 tmp;

	/*
	* Accel configuration
	*/

	/* CTRL_REG1_A: low power disabled, 400Hz update rate, all axes enabled */
	tmp = 0x97;
	if(i2c_write(file, ACCEL_ADDRESS, ACCEL_CTRL_REG1_A, 1, &tmp) != 1) {
		printf("Unable to write CTRL_REG1A\n");
	}
	
	/* CTRL_REG4_A: scale, high res update mode */
	tmp = 0x08;
	if(i2c_write(file, ACCEL_ADDRESS, ACCEL_CTRL_REG4_A, 1, &tmp) != 1) {
		printf("Unable to write CTRL_REG4A\n");
	}

	/*
	* Compass configuration
	*/

	/* CRA_REG_M: temp sensor on, update rate set to 220Hz */
	tmp = 0x9C;
	if(i2c_write(file, MAGNET_ADDRESS, MAGNET_CRA_REG_M, 1, &tmp) != 1) {
		printf("Unable to write CRA_REG_M\n");
	}

	/* CRB_REG_M: gain setting */
	tmp = 0xE0;
	if(i2c_write(file, MAGNET_ADDRESS, MAGNET_CRB_REG_M, 1, &tmp) != 1) {
		printf("Unable to write CRB_REG_M\n");
	}

	/* MR_REG_M continous-conversion mode */
	tmp = 0x00;
	if(i2c_write(file, MAGNET_ADDRESS, MAGNET_MR_REG_M, 1, &tmp) != 1) {
		printf("Unable to write MR_REG_M\n");
	}

	/*
	* Gyro configuration
	*/

	/* CTRL_REG1 ODR, bandwidth, power down off, all axes enabled  */
	tmp = 0xEF;
	if(i2c_write(file, GYRO_ADDRESS, GYRO_CTRL_REG1, 1, &tmp) != 1) {
		printf("Unable to write CTRL_REG1_A\n");
	}

	/* CTRL_REG4 ODR, full scale  */
	tmp = 0x10;
	if(i2c_write(file, GYRO_ADDRESS, GYRO_CTRL_REG4, 1, &tmp) != 1) {
		printf("Unable to write CTRL_REG1_A\n");
	}

	__u8 accel_axes[6];
	__u8 magnet_axes[6];
	__u8 gyro_axes[6];

	__s16 accel_values[3];
	__s16 magnet_values[3];
	__s16 gyro_values[3];


	while(1) {
		if(i2c_read(file, ACCEL_ADDRESS, ACCEL_AXES_REG, 6, accel_axes) != 6) {
			printf("Unable to read from accel device\n");
		}

		accel_values[0] = (__s16)(accel_axes[1]<<8 | accel_axes[0]);
		accel_values[1] = (__s16)(accel_axes[3]<<8 | accel_axes[2]);
		accel_values[2] = (__s16)(accel_axes[5]<<8 | accel_axes[4]);
	
		if(i2c_read(file, MAGNET_ADDRESS, MAGNET_AXES_REG, 6, magnet_axes) != 6) {
			printf("Unable to read from magnet device\n");

		}

		magnet_values[0] = (__s16)(magnet_axes[1]<<8 | magnet_axes[0]);
		magnet_values[2] = (__s16)(magnet_axes[3]<<8 | magnet_axes[2]);
		magnet_values[1] = (__s16)(magnet_axes[5]<<8 | magnet_axes[4]);
		
		if(i2c_read(file, GYRO_ADDRESS, GYRO_AXES_REG, 6, gyro_axes) != 6) {
			printf("Unable to read from gyro device\n");
		}

		gyro_values[0] = (__s16)(gyro_axes[1]<<8 | gyro_axes[0]);
		gyro_values[1] = (__s16)(gyro_axes[3]<<8 | gyro_axes[2]);
		gyro_values[2] = (__s16)(gyro_axes[5]<<8 | gyro_axes[4]);

		printf("a: %06hi, %06hi, %06hi m: %06hi, %06hi, %06hi g:%06hi, %06hi, %06hi \n", 
			accel_values[0], accel_values[1], accel_values[2],
			magnet_values[0], magnet_values[1], magnet_values[2],
			gyro_values[0], gyro_values[1], gyro_values[2]);		

	}

	if (i2c_close(file) != 0) {
		printf("Unable to close i2c bus.\n");
	}

	return 0;
}
