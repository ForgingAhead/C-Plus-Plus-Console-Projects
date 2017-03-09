/*******************************

clientAppLab3.cpp:.
A simple client in the internet domain using TCP.
The port number is specified as 3745.
The server's IP address is 127.0.0.1.

Functions included:
void error(const char *msg); a function for error handling; the parameter is a string describing the error.
int main(); no parameter needed.

Author: Chen Feng

***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <Winsock2.h>
#include <io.h>
#include <string.h>
#define MAXBUFSIZE 256
#define PORTNO 3745
#define HOSTADDR "127.0.0.1"

/************** error handling */
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    char buffer[MAXBUFSIZE];
/*************** initializing socket version etc.*/
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(1,1);
    err = WSAStartup(wVersionRequested,&wsaData);
    if(err!=0)
    {
        return 0;
    }
	if(LOBYTE(wsaData.wVersion)!=1 || HIBYTE(wsaData.wVersion)!=1)
     {
        WSACleanup();
        return 0;
    }

/************* create a socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

/************* specify the server's IP address */
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.S_un.S_addr = inet_addr(HOSTADDR);
    serv_addr.sin_port = htons(PORTNO);

/************* connect */
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

/************* send a message to server */
	for(int k=0; k<2; k++)
	{
	for(int i=0; i<MAXBUFSIZE; i++)
		buffer[i] = '\0'; // be zero
    printf("Please enter the message: ");
	gets(buffer);
    n = send(sockfd,buffer,strlen(buffer),0);
    if (n < 0)
         error("ERROR writing to socket");


/************** receive the message from server */
	for(int j=0; j<MAXBUFSIZE; j++)
		buffer[j] = '\0'; // be zero
    n = recv(sockfd,buffer,MAXBUFSIZE,0);
    if (n < 0)
         error("ERROR reading from socket");
    printf("The local time on host is: %s\n",buffer);
	}
    close(sockfd);
    return 0;
}
