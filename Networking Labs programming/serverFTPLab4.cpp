/*******************************

serverFTPLab4.cpp:.
A simple server in the internet domain using TCP to realize file transfer.
The port number is specified as 3745.

Functions included:
void error(const char *msg); a function for error handling; the parameter is a string describing the error.
int main(); no parameter needed.
DWORD WINAPI ft_thread(void * socketfd); dealing with the file transfer work( send & receive )

Author: Chen Feng

***************************************/
#include<stdio.h>
#include<Winsock2.h>
#include<memory.h>
#include<io.h>
#include<string.h>
#include<process.h>
#define MAXBUFSIZE 1024
#define PORTNO 3555
#pragma comment(lib, "ws2_32.lib")

/***************** error handling */
void error(const char *msg)
{
    perror(msg);
    //exit(1);
}

DWORD WINAPI ft_thread(void * socketfd);

int main(int argc, char *argv[])
{
     int sockfd, newsockfd;
     int clilen;
     struct sockaddr_in serv_addr, cli_addr;
/****************** initializing socket version etc. */
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

/************* create listen socket & bind an IP address to the socket */
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
	 //printf("%d\n",sockfd);////////////////////////
     if (sockfd < 0)
        error("ERROR opening socket");
     bind(sockfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));

/*************** specify the local address (the server's) */
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(PORTNO);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");

/**************** listen & accept */

     listen(sockfd,5);
     clilen = sizeof(cli_addr);
	printf("waiting for request....\n");
	 while(1)
	 {
		 newsockfd = accept(sockfd,
					 (struct sockaddr *) &cli_addr,
					 &clilen);
		 //printf("accept........\n");////////////////////////testing
		 if (newsockfd < 0)
			  error("ERROR on accept");
		unsigned long threadID;
		if(CreateThread(NULL,0,&ft_thread,(void *)newsockfd,0,&threadID) == NULL)
			error("ERROR on creating thread");
	 }
     close(sockfd);
     return 0;
}

DWORD WINAPI ft_thread(void * sock)
{
	//printf("in the thread\n");/////////////////testing
	char buffer[MAXBUFSIZE];
	int len = 256;
	int n = 0;
	char file[200];
	char opt[200];//specify the operation
	int filelen = 0;
/*************** receive msg from client and send file(s) to client */
	while(1)
	{
		memset(buffer, 0, MAXBUFSIZE);
		memset(file, 0, 200);
		//printf("in the while loop\n");//////////////////////testing
		if((n = recv((int)sock, buffer, MAXBUFSIZE, 0)) == -1)
		{//if(n == -1£© indicates the thread closed
			printf("close the thread\n");
			return 0;
		}
		//printf("%s ----buffer\n", buffer);////////////////testing
		sscanf(buffer, "%s %s", opt, file);//to check whether its a msg asking for file transfer
		if(strcmp(opt, "download") == 0)
		{
			//printf("here for tesing 1 \n");/////////////////////testing
			FILE* fl;
			fl = fopen(file, "rb");
			if(fl == NULL)
			{// failed to open the file
				sprintf(buffer, "-1\n");
				n = send((int)sock, buffer, strlen(buffer), 0);
				if(n < 0)
				{
					error("sending error");
					continue;
				}
				printf("File %s is not found\n", file);
				continue;
			}
			else
			{// succeeded in opening the file
				filelen = filelength(fileno(fl));
				sprintf(buffer, "%d bytes of the file", filelen);
				n = send((int)sock, buffer, strlen(buffer), 0);
				if(n < 0)
				{
					error("sending file error");
					continue;
				}
				int remained = filelen;//length of the remained file for transfering
				printf("%s transfer begins.......total length %d bytes\n", file, filelen);
				while(remained > 0)
				{

					int tmp = fread(buffer, sizeof(char), MAXBUFSIZE, fl);
					if(send((int)sock, buffer, tmp, 0) < 0)
					{
						error("sending file error");
						continue;
					}
					remained -= tmp;
				}
				fclose(fl);
				printf("%s transfer completed\n", file);
			}
		}
		else
			close((int)sock);
	}
	 close((int)sock);
	 return 0;
}
