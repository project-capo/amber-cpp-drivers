#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <linux/types.h>
#include "RoboclawLib.h"
#include <termios.h>

void read_firmware_version(int fd, __u8 address);
void read_speed(int fd, __u8 address);
void read_battery(int fd, __u8 address);

void read_firmware_version(int fd, __u8 address) {
	__u8 buf[100];

	printf ("%d: read firmware version:\n", address);
	if (rc_read_firmware_version(fd, address, buf) <= 0) {
		printf("%d: error.\n", address);
		return;
	}

	printf("%d: %s\n", address, buf);
}

void read_speed(int fd, __u8 address) {
	unsigned int m1_value, m2_value;
	__u8 m1_dir, m2_dir;

	printf ("%d: read speed:\n", address);

	if (rc_read_speed_m1(fd, address, &m1_value, &m1_dir) == -1) {
		printf("%d: m1_speed error.\n", address);
		return;
	}

	if (rc_read_speed_m2(fd, address, &m2_value, &m2_dir) == -1) {
		printf("%d: m2_speed error.\n", address);
		return;
	}

	printf("%d: m1_speed: %d, m1_dir: %d, m2_speed: %d, m2_dir = %d\n", 
		address, m1_value, m1_dir, m2_value, m2_dir);
}

void read_battery(int fd, __u8 address) {
	__u16 voltage;

	printf ("%d: read main battery:\n", address);

	if (rc_read_main_battery_voltage_level(fd, address, &voltage) == -1) {
		printf("%d: read main battery: error.\n", address);
		return;
	}

	printf("%d: main_battery: %d\n", 
		address, voltage);
}


int main() {

	int fd;
 
	fd = rc_uart_open("/dev/ttyO3");
	rc_uart_init(fd, B38400);

	read_firmware_version(fd, 128);
	read_firmware_version(fd, 129);

	read_speed(fd, 128);
	read_speed(fd, 129);	

	read_battery(fd, 128);
	read_battery(fd, 129);

	rc_uart_close(fd);
	return 0;
}
 