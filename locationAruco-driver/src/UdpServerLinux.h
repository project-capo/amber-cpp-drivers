/*
 * UdpServerLinux.h
 *
 *  Created on: Jun 13, 2017
 *      Author: szsz
 */

#ifndef UDPSERVERLINUX_H_
#define UDPSERVERLINUX_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define BUF_SIZE 500

class UdpServerLinux {
private:
	int fd;
	 struct sockaddr_in serveraddr;
	 char buffer[BUF_SIZE];


public:
	UdpServerLinux(unsigned short port);
	virtual ~UdpServerLinux();
	int ReciveBufLen;
	char* Receive();
};

#endif /* UDPSERVERLINUX_H_ */
