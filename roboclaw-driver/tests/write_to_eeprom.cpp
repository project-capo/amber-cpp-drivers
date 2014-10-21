#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <linux/types.h>
#include "RoboclawLib.h"
#include <termios.h>

void write_to_eeprom(int, __u8);

void write_to_eeprom(int fd, __u8 address) {
	printf ("%d: writing to eeprom\n", address);

	if (rc_write_to_eeprom(fd, address) == -1) {
		printf("%d: error\n", address);
		return;
	} else {
		printf("%d: ok\n", address);
	}
}
	
int main() {

	int fd;
 
	fd = rc_uart_open("/dev/ttyO3");
	rc_uart_init(fd, B38400);

	write_to_eeprom(fd, 128);
	write_to_eeprom(fd, 129);

	rc_uart_close(fd);
	return 0;
}
 