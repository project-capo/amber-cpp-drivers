#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <linux/types.h>
#include "RoboclawLib.h"
#include <termios.h>

#include <boost/thread.hpp>
#include <boost/thread/thread_time.hpp>

int main(int argc, char *argv[]) {

        int gpio_fd, fd;

        if (argc != 3) {
        	printf("reset_and_go [claw_address] [delay]");
        	return 1;
        }

        gpio_fd = rc_gpio_open("/sys/class/gpio/gpio136/value");

        fd = rc_uart_open("/dev/ttyO3");
        rc_uart_init(fd, B38400);

        if (rc_reset(gpio_fd) == 0) {               
    		boost::this_thread::sleep(boost::posix_time::milliseconds(atoi(argv[2])));

	        rc_drive_forward(fd, (__u8)atoi(argv[1]), 50);

	        boost::this_thread::sleep(boost::posix_time::seconds(1));

	        rc_drive_forward(fd, (__u8)atoi(argv[1]), 0);

           	printf("reset and go done\n");
        } else {
            printf("reset failed\n");
            return 1;
        }

        return 0;
}                     