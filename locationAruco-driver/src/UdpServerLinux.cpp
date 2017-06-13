/*
 * UdpServerLinux.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: szsz
 */

#include "UdpServerLinux.h"

UdpServerLinux::UdpServerLinux(unsigned short port) {
	// TODO Auto-generated constructor stub

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror( "socket failed" );
    }

    memset( &serveraddr, 0, sizeof(serveraddr) );
    		    serveraddr.sin_family = AF_INET;
    		    serveraddr.sin_port = htons( port );
    		    serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );

   if ( bind(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 ) {
    		   		        perror( "bind failed" );
    		   		    }
}

UdpServerLinux::~UdpServerLinux() {
	// TODO Auto-generated destructor stub
	close( fd );
}

char* UdpServerLinux::Receive()
{
	ReciveBufLen = recvfrom( fd, buffer, sizeof(buffer) - 1, 0, NULL, 0 );

return buffer;
}
