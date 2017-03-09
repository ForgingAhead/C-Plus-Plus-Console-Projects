/*******************************

clientFTPLab4.cpp:.
A simple client in the internet domain using TCP to realize file transfer.
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
#include<memory.h>
#define MAXBUFSIZE 1024
#define PORTNO 3555
#define HOSTADDR "127.0.0.1"
#pragma comment(lib, "ws2_32.lib")

/************** error handling */
void error(const char *msg)
{
    perror(msg);
    //exit(0);
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

/************* create a socket ****/ 
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
     
	char file[200];
	int remained;
	FILE* fl;
	int result;
	int num, i;
	while(true)
	{
		memset(file, 0, 200);
		memset(buffer, 0, MAXBUFSIZE);// be zero
		printf("Please enter the full file name you want to download: "); 
		gets(file);
		sprintf(buffer, "download  %s", file);
		//printf("buffer:%s\n",buffer);////////////tesing
		if(send(sockfd,buffer,strlen(buffer),0) < 0)
		{
			 error("ERROR writing to socket");
			 continue;
		}
		//printf("here for tesing 1 \n");/////////////////////testing
		memset(buffer, 0, MAXBUFSIZE);
		if((num = recv(sockfd, buffer, MAXBUFSIZE, 0)) < 0)
		{
			error("error in receiving\n");
			continue;
		}
		//printf("here>>>>>>\n");//////////////testing
		sscanf(buffer, "%d", &result);
		if(result < 0)
		{
			error("no such file!");
			continue;
		}
		fl = fopen(file, "wb");
		//printf("here for tesing 2 \n");/////////////////////testing
		fwrite(buffer, sizeof(char), num, fl);
		//printf("here for tesing 3 \n");/////////////////////testing
		printf("%s transfer begins, total length %d bytes.\n",file, result);
		result -= num;
		while(result > 0)
		{
			memset(buffer, 0, sizeof(buffer) );
			result -= (num = recv(sockfd, buffer, MAXBUFSIZE, 0) );
			fwrite(buffer, sizeof(char), num, fl);
		}
		fclose(fl);
		printf("%s transfer completed!\n", file);
	}
    close(sockfd);
    return 0;
}
