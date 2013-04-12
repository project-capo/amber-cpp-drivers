#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <linux/types.h>
#include "uart.h"
#include "roboclaw_lib.h"

int main(int argc, char *argv[]) {

	int fd;

	if (argc < 3) {
		printf("speedL speedR\n");
		return 1;	
	}

	int speedL = atoi(argv[1]);
	int speedR = atoi(argv[2]);
	unsigned int qpps = 13800;
	unsigned int p = 65536;
	unsigned int i = 32768;
	unsigned int d = 16384;

	fd = uart_open("/dev/ttyO3");
	uart_init(fd, B38400);

	rc_set_pid_consts_m1(fd, 128, d, p, i, qpps);
	rc_set_pid_consts_m2(fd, 128, d, p, i, qpps);
	
	rc_set_pid_consts_m1(fd, 129, d, p, i, qpps);
	rc_set_pid_consts_m2(fd, 129, d, p, i, qpps);

	//rc_buffered_drive_speed_dist(fd, 129, 5000, 1000000, 5000, 1000000, 1);
	//rc_buffered_drive_speed_dist(fd, 129, 5000, 1000000, 5000, 1000000, 1);

	rc_drive_speed(fd, 128, speedL, speedR);
	rc_drive_speed(fd, 129, speedL, speedR);

	//rc_drive_forward_m1(fd, 128, 30);
	//rc_drive_forward(fd, 128, 20);
	//rc_drive_forward(fd, 129, 20);

	//rc_drive_speed_accel(fd, 128, 1000, speedL, speedR);
	//rc_drive_speed_accel(fd, 129, 1000, speedR, speedL);

	uart_close(fd);
	
	return 0;
}
