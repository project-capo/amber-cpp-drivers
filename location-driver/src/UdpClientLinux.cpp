#include "UdpClientLinux.h"

UdpClient::UdpClient(char* sIP,unsigned short portNo,unsigned int bufforSize)
{
    len = sizeof(struct sockaddr_in);
    len2 = sizeof(struct sockaddr_in);

    host = gethostbyname(sIP);
    port = portNo;

    /* initialize socket */
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)  
	err((char*)"socket");

    /* initialize server addr */
    memset((char *) &server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr = *((struct in_addr*) host->h_addr);

    struct timeval tv;
    tv.tv_sec = TIMEOUT_S;
    tv.tv_usec = 100000;

    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0)
     	 err((char*)"Error timeout");
}

UdpClient::~UdpClient()
{
    close(s);
}

char* UdpClient::Receive()
{
	n = recvfrom(s, buf, BUF_SIZE, 0, (struct sockaddr *) &server, &len2);

	return buf;
}

void UdpClient::Send(const char* dgram,int count)
{ 
   /* send message */
   if (sendto(s, dgram, count, 0, (struct sockaddr *) &server, len2) == -1) 
	err((char*)"sendto()");
}

void UdpClient::err(char *s)
{
    perror(s);
    exit(1);
}
