#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define BUF_SIZE 7168
#define TIMEOUT_S      2     /* Seconds between retransmits */

class UdpClient
{
public:
UdpClient(char* sIP,unsigned short port,unsigned int bufforSize);
~ UdpClient();

char* Receive();
void Send(const char* dgram,int count);

int n;

private:
    struct sockaddr_in server;
    int len;
    socklen_t len2; 
    char buf[BUF_SIZE];
    struct hostent *host;
    int  s, port;
	

    void err(char *s);

};


