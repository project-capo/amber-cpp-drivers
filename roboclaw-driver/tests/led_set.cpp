#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <linux/types.h>
#include "RoboclawLib.h"
#include <termios.h>

#define LED1_GPIO_PATH "/sys/class/gpio/gpio139/value"
#define LED2_GPIO_PATH "/sys/class/gpio/gpio137/value"

void led_set(int gpio_fd, bool state);
void print_usage();

void led_set(int gpio_fd, bool state) {
	
	if (rc_gpio_set(gpio_fd, state) < 0) {
		printf("rc_gpio_set: error\n");
	} else {
		printf("rc_gpio_set: ok\n");
	}
}

void print_usage() {
	printf("usage: led_id [1 2], state [0 1]");
}

int main(int argc, char* argv[]) {

	if (argc != 3) {
		print_usage();
		return 1;
	}

	int led_id = atoi(argv[1]);
	int state = atoi(argv[2]);

	if ((led_id != 1 && led_id != 2) || (state != 0 && state != 1)) {
		print_usage();
		return 1;
	}

	int gpio_fd;

	if (led_id == 1) {
		gpio_fd = rc_gpio_open(LED1_GPIO_PATH);
	} else if (led_id == 1) {
		gpio_fd = rc_gpio_open(LED2_GPIO_PATH);
	}
		
 	led_set(gpio_fd, state == 1 ? true : false);

	return 0;
}	