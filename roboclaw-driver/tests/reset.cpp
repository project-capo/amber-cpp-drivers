#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <linux/types.h>
#include "RoboclawLib.h"
#include <termios.h>

int main() {

	int gpio_fd;
 
	gpio_fd = rc_gpio_open("/sys/class/gpio/gpio136/value");
	if (rc_reset(gpio_fd) == 0) {
		printf("reset done\n");
	} else {
		printf("reset failed\n");
	}

	return 0;
}