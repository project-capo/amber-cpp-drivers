#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define BUF_SIZE1 10000
#define TIMEOUT_S      2     /* Seconds between retransmits */
#define TIMEOUT_MSS    100000     /* Seconds between retransmits */

class UdpClient
{
public:
UdpClient(char* sIP,unsigned short port);
~ UdpClient();

char* Receive();
void Send(const char* dgram,int count);

int n;

private:
    struct sockaddr_in server;
    int len;
    socklen_t len2; 
    char buf[BUF_SIZE1];
    struct hostent *host;
    int  s;
    unsigned short port;
	

    void err(const char *s);

};


