#ifndef HITEC_H_
#define HITEC_H_

#include <linux/types.h>
#include <termios.h>

int ht_uart_open(const char *blockdevice);
int ht_uart_init(int fd, speed_t speed);
ssize_t ht_uart_write(int fd, int bytes, __u8 *buf);
ssize_t ht_uart_read(int fd, int to_read, __u8 *buf);
int ht_uart_flush_input(int fd);
int ht_uart_close(int fd);

int ht_set_angle(int fd, unsigned int servo_address, int angle);
int ht_set_speed(int fd, unsigned int servo_address, int speed);
int ht_set_same_angle(int fd, unsigned int *servo_addresses, int servo_count, int angle);
int ht_set_different_angles(int fd, unsigned int *servo_addresses, int servo_count, int *angles);

int ht_check_angle(int angle);
int ht_check_speed(int speed);
void ht_prepare_command(int angle, __u8 *buffer);
void ht_prepare_command_speed(int angle, __u8 *buffer);

#endif
