/*******************************

serverAppLab1.cpp:.
A simple server in the internet domain using TCP.
The port number is specified as 3745.

Functions included:
void error(const char *msg); a function for error handling; the parameter is a string describing the error.
int main(); no parameter needed.

Author: Chen Feng

***************************************/
#include<stdio.h>
#include<Winsock2.h>
#include<time.h>
#include<io.h>
#include<string.h>
#define MAXBUFSIZE 256
#define PORTNO 3745

/************ error handling **************/
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd;
     int clilen;
     char buffer[MAXBUFSIZE];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
/********** initializing socket version etc. ***************/
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

/************** create listen socket & bind an IP address to the socket */
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
	 printf("%d\n",sockfd);////////////////////////
     if (sockfd < 0)
        error("ERROR opening socket");

/************** specify the local address (the server's) */
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(PORTNO);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");

/***************** listen & accept */

     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");

/**************** read message from client */
	for(int i=0; i<MAXBUFSIZE; i++)
		buffer[i] = '\0'; // be zero
     n = recv(newsockfd,buffer,MAXBUFSIZE,0);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message from client: %s\n",buffer);

/**************** send local time to client */
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	char *localtime = asctime (timeinfo);
     n = send(newsockfd,localtime,strlen(localtime),0);
     if (n < 0) error("ERROR writing to socket");

     close(newsockfd);
     close(sockfd);
     return 0;
}
