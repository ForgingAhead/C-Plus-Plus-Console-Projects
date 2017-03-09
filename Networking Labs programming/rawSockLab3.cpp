/******************************

Using SOCK_RAW to realize sniffer-like functionality

Functions included:
void error(const char *msg); a function for error handling; the parameter is a string describing the error.
int main(); no parameter needed.

Author: Chen Feng

********************************/

#include<stdio.h>
#include<Winsock2.h>
#include<memory.h>
#include<io.h>
#include<string.h>

#define SIO_RCVALL            _WSAIOW(IOC_VENDOR,1)
#define SIO_RCVALL_IGMPMCAST  _WSAIOW(IOC_VENDOR,3)
#define MAXBUFSIZE 10000
#define SOURCE_PORT 4537
#define DEST_PORT 3745
#define PROTOCOL_STRING_ICMP_TXT "ICMP" 
#define PROTOCOL_STRING_TCP_TXT "TCP" 
#define PROTOCOL_STRING_UDP_TXT "UDP" 
#define PROTOCOL_STRING_SPX_TXT "SPX" 
#define PROTOCOL_STRING_NCP_TXT "NCP" 
#define PROTOCOL_STRING_UNKNOW_TXT "UNKNOWN"

char* getProtocolTxt(int Protocol);

/***************** error handling *********/
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

typedef struct _TCP
{
    unsigned short   tcp_sport;// source port
    unsigned short   tcp_dport; //destination port
    unsigned int     tcp_seq;//sequence number
    unsigned int     tcp_ack;//acknowledge number
    unsigned char    tcp_lenres;//
    unsigned char    tcp_flag;//flag
    unsigned short   tcp_win;//window size  
    unsigned short   tcp_sum;//check sum 
    unsigned short   tcp_urp;      
}TCP;

typedef struct _IP
{ 
	union
	{ 
		BYTE Version; // 版本 
		BYTE HdrLen; // IHL 
	}; 
	BYTE ServiceType; // 服务类型 
	WORD TotalLen; // 总长 
	WORD ID; // 标识 
	union
	{ 
		WORD Flags; // 标志 
		WORD FragOff; // 分段偏移 
	}; 
	BYTE TimeToLive; // 生命期 
	BYTE Protocol; // 协议 
	WORD HdrChksum; // 头校验和 
	DWORD SrcAddr; // 源地址 
	DWORD DstAddr; // 目的地址 
	BYTE Options; // 选项 
}IP; 


int main()
{
	int sniffersock;
	char buffer[MAXBUFSIZE];
	bool blnFlag = true;
	TCP tcp;
	IP ip;
/****************** initializing socket version etc. *********/
	WORD wVersionRequested;   
    WSADATA wsaData;    
    int err;   
    wVersionRequested = MAKEWORD(2,2);   
    err = WSAStartup(wVersionRequested,&wsaData);    
    if(err!=0)    
    { 
        return 0;    
	if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2)    
     {    
        WSACleanup();    
        return 0;     
    }
    }   
	
	sniffersock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if(sniffersock < 0)
		error("ERROR opening socket");
	
	//if(setsockopt(sniffersock, IPPROTO_IP, IP_HDRINCL, (char *)&blnFlag, sizeof(blnFlag)) == SOCKET_ERROR)
		//error("setsockopt IP_HDRINCL error"); // set the sniffersock

/************** fill struct SOCKADDR_IN***/
	 char hostName[128];
	 gethostname(hostName,128);
	 hostent *pHostIP;
	 pHostIP = gethostbyname(hostName);

	 sockaddr_in local_addr;
	 local_addr.sin_family = AF_INET;
	 local_addr.sin_port = htons(6565);
	 local_addr.sin_addr = *(in_addr*)pHostIP->h_addr;

/************* bind raw socket to local network address**************/
	if(bind(sniffersock,(sockaddr *)&local_addr, sizeof(struct sockaddr)) == SOCKET_ERROR)
		error("ERROR on binding");
/************** set the SOCK_RAW to SIO_RCVALL to receive all IP package ****************/
	DWORD dwValue = 1;
	ioctlsocket(sniffersock, SIO_RCVALL, &dwValue);
	printf("Listening......\n");
	while(1)
	{
		memset(buffer, '\0', MAXBUFSIZE); //be zero
		//printf("in the while loop\n");/////////////////for testing
		struct sockaddr from;
		int from_len = (int)sizeof(struct sockaddr);
		int ret = recvfrom(sniffersock, buffer, MAXBUFSIZE, 0, (sockaddr *)&from, &from_len); 
		//printf("Bytes received %d \n",ret);//////////////for testing
		if (ret > 0) 
		{ 
/********** Analyse the received packet *********/

			ip = *(IP*)buffer;
			tcp = *(TCP*)(buffer + ip.HdrLen); 
			if(strcmp((char *)inet_ntoa(*(in_addr*)&ip.SrcAddr), "192.168.1.53") == 0)//?????????????
			{ 				
				printf("package length: %d\r\n",ip.TotalLen);
				printf("IP source address: %s\r\n",inet_ntoa(*(in_addr*)&ip.SrcAddr)); 
				printf("IP destination address: %s\r\n",inet_ntoa(*(in_addr*)&ip.DstAddr)); 
				printf("TCP source port: %d\r\n",tcp.tcp_sport); 
				printf("TCP destination port:%d\r\n",tcp.tcp_dport); 
				printf("IP header length: %d\r\n",ip.HdrLen & 0x0F);
				printf("Protocol: %s\r\n\n\n",getProtocolTxt(ip.Protocol));
			}
		}
	}
	return 0;
} 

/**
getProtocalTxt: return the name of the protocol specified by the parameter
  **/
char* getProtocolTxt(int Protocol) 
{ 
	switch (Protocol){ 
	case IPPROTO_ICMP : /* control message protocol */ 
		return PROTOCOL_STRING_ICMP_TXT; 
	case IPPROTO_TCP : /* tcp */ 
		return PROTOCOL_STRING_TCP_TXT; 
	case IPPROTO_UDP : /* user datagram protocol */ 
		return PROTOCOL_STRING_UDP_TXT; 
	default: 
		return PROTOCOL_STRING_UNKNOW_TXT; 
	}
}

