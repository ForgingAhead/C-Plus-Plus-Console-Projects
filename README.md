# C-Plus-Plus-Console-Projects
Library management system, console-based. 
Calculating time and date App - console-based.
Networking Labs, communicating between client and servers, file tranfer...


Notes:
/获取本机IP地址
 char hostName[128];
 gethostname(hostName,128);
 hostent *pHostIP;
 pHostIP = gethostbyname(hostName);
 
 //填充sockaddr_in结构体
 sockaddr_in localAddr;
 localAddr.sin_family = AF_INET;
 localAddr.sin_port = htons(6000);
 localAddr.sin_addr = *(in_addr*)pHostIP->h_addr;
