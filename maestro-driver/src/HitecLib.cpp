#include <cstdio>
#include <unistd.h>
#include <linux/types.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <strings.h>

#include <boost/thread.hpp>
#include <boost/thread/thread_time.hpp>

#include "HitecLib.h"

int ht_uart_open(const char *blockdevice) {
	return open(blockdevice, O_RDWR);
}

int ht_uart_close(int fd) {
	return close(fd);
}

int ht_uart_init(int fd, speed_t speed)
{
    struct termios termios;

    bzero(&termios, sizeof(termios));
    cfmakeraw(&termios);

    termios.c_cflag |= (CS8 | CREAD | CLOCAL);

    termios.c_cc[VMIN] = 0;
    termios.c_cc[VTIME] = 1;

    cfsetispeed(&termios, speed);
    cfsetospeed(&termios, speed);

    return tcsetattr(fd, TCSANOW, &termios);
}

ssize_t ht_uart_write(int fd, int bytes, __u8 *buf) {
    ssize_t res, chars_in_tx_queue;

    res = write(fd, buf, bytes);
    if (res < 0) {
        return -1;
    }

    // wait for buffer to empty
    do {
    	boost::this_thread::sleep(boost::posix_time::microseconds(500));
    	ioctl (fd, TIOCOUTQ, &chars_in_tx_queue);
    } while (chars_in_tx_queue > 0);

    //tcdrain(fd);

    return res;
}

ssize_t ht_uart_read(int fd, int to_read, __u8 *buf) {
	ssize_t received;
    ssize_t sum = 0;

    while (sum < to_read) {
    	received = read(fd, buf, 1);
    	buf += received;
    	sum += received;

    	if (received <= 0) {
    		return sum;
    	}
    }

    return sum;
}

int ht_uart_flush_input(int fd) {
     return tcflush(fd, TCIFLUSH);
}

int ht_set_angle(int fd, unsigned int servo_address, int angle) {
	if(ht_check_angle(angle) < 0) {
		return -1;
	}

	const ssize_t command_size = 4;

    __u8 buffer[command_size];
    ht_prepare_command(angle, buffer);
    buffer[1] = static_cast<__u8>(servo_address & 0xFF);

    if (ht_uart_write(fd, command_size, buffer) != command_size) {
    	return -1;
	}

	return 0;
}

int ht_set_speed(int fd, unsigned int servo_address, int speed) {
	if(ht_check_speed(speed) < 0) {
		return -1;
	}

	const ssize_t command_size = 4;

    __u8 buffer[command_size];
    ht_prepare_command_speed(speed, buffer);
    buffer[1] = static_cast<__u8>(servo_address & 0xFF);

    if (ht_uart_write(fd, command_size, buffer) != command_size) {
    	return -1;
	}

	return 0;
}

int ht_set_same_angle(int fd, unsigned int *servo_addresses, int servo_count, int angle) {
	if(ht_check_angle(angle) < 0) {
		return -1;
	}

	const ssize_t command_size = 4;
    __u8 buffer[command_size];
    ht_prepare_command(angle, buffer);

    for(int i = 0; i < servo_count; i++) {
    	buffer[1] = static_cast<__u8>(*(servo_addresses + i) & 0xFF);

        if (ht_uart_write(fd, command_size, buffer) != command_size) {
        	return -1;
    	}
    }

    return 0;
}

int ht_set_different_angles(int fd, unsigned int *servo_addresses, int servo_count, int *angles) {
	for(int i = 0; i < servo_count; i++) {
		if(ht_check_angle(*(angles + i)) < 0) {
			return -1;
		}
	}

	for(int i = 0; i < servo_count; i++) {
		int servo_address = *(servo_addresses + i);
		int angle = *(angles + i);
		int return_value = ht_set_angle(fd, servo_address, angle);

		if(return_value < 0) {
			return -1;
		}
	}

	return 0;
}

void ht_prepare_command(int angle, __u8 *buffer) {
	const int max_sec_value = 2400;
	const int degree_in_usec = 10;

	int data_value = (max_sec_value - degree_in_usec * angle) * 4;

	const __u8 command_byte = 0x84;
	const __u8 data_byte_I = data_value & 0x7F;
	const __u8 data_byte_II = data_value >> 7 & 0x7F;

    buffer[0] = command_byte;
    buffer[2] = data_byte_I;
    buffer[3] = data_byte_II;
}

void ht_prepare_command_speed(int speed, __u8 *buffer) {


	int data_value = speed;

	const __u8 command_byte = 0x87;
	const __u8 data_byte_I = data_value & 0x7F;
	const __u8 data_byte_II = data_value >> 7 & 0x7F;

    buffer[0] = command_byte;
    buffer[2] = data_byte_I;
    buffer[3] = data_byte_II;
}

int ht_check_angle(int angle) {
	if(angle > 180 || angle < 0) {
		return -1;
	}

	return 0;
}
int ht_check_speed(int speed) {
	if(speed > 255 || speed < 0) {
		return -1;
	}

	return 0;
}

