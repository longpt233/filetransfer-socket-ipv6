//NOTE : compile with -lws2_32   option 
// client on windows

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>

//#include <arpa/inet.h>    // inet_pton
//#include <netinet/in.h>
//#include <sys/socket.h>

//#include<windows.h>
#include<winsock2.h>
#include<ws2tcpip.h>

#define MAX_LINE 4096
#define LINSTENPORT 7788
#define SERVERPORT 8821
#define BUFFSIZE 4096

void sendfile(FILE *fp, int sockfd);
void sendtext(int sockfd);
int inet_pton(int af, const char *src, void *dst);

int main(){

    // init 
    WSADATA wsa;
    WORD wVer=MAKEWORD(2,2);
    if(WSAStartup(wVer,&wsa)) 
        printf("version not suppoted");

    // socket
    SOCKET sockfd;
   // sockfd = socket(AF_INET, SOCK_STREAM, 0);
     sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Can't allocate sockfd");
        exit(1);
    }

    struct sockaddr_in6 serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;
    serveraddr.sin6_port = htons(SERVERPORT);
    //serveraddr.sin_addr.s_addr=inet_addr("192.168.1.9");
    //char ipstr[]="fe80:b593:29c6:8829:2ff9";;2001:ee0:43b0:9f80:790f:6df6:384b:e066
    // neu cung mot may thi gan dai chi ntn cx dc??
     if (inet_pton(AF_INET6, "::1", &serveraddr.sin6_addr) < 0){  // convert to 6: AF_INET6
        perror("IPaddress Convert Error");
         exit(1);
     }  // windown k co ham nay 

    //printf()
    if (connect(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0){
        perror("Connect Error\n");
        printf("%d\nend",WSAGetLastError);
        return EXIT_FAILURE;
    }
    else{
        printf("connected\n");
    }
   
    
    sendtext(sockfd);
    
    FILE *fp = fopen("E:\\testv4\\mes.txt", "rb");// window phai co "\\" :unknown escape sequence
    if (fp == NULL) {
        perror("Can't open file");
        exit(1);
    }
    sendfile(fp, sockfd);
    fclose(fp);


    closesocket(sockfd);
    WSACleanup();
    return 0;
}

void sendfile(FILE *fp, int sockfd) {
    int n; 
    char sendline[MAX_LINE] = {0}; 
    ssize_t total=0;
    while ((n = fread(sendline, sizeof(char), MAX_LINE, fp)) > 0) {
	    total+=n;
        if (n != MAX_LINE && ferror(fp)) {
            perror("Read File Error");
            exit(1);
        }
        
        if (send(sockfd, sendline, n, 0) == -1){
            perror("Can't send file");
            exit(1);
        }
        memset(sendline, 0, MAX_LINE);
    }
    
     printf("Send Success, NumBytes = %ld\n", total);
}


void sendtext(int sockfd){
     char buff[BUFFSIZE] = "hello";
    if (send(sockfd, buff, BUFFSIZE, 0) == -1){
        perror("Can't send filename");
        exit(1);
    }else{
        printf("sent sucsessfully !\n");
    }
};

int inet_pton(int af, const char *src, void *dst){
  struct sockaddr_storage ss;
  int size = sizeof(ss);
  char src_copy[INET6_ADDRSTRLEN+1];

  ZeroMemory(&ss, sizeof(ss));
  /* stupid non-const API */
  strncpy (src_copy, src, INET6_ADDRSTRLEN+1);
  src_copy[INET6_ADDRSTRLEN] = 0;

  if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
    switch(af) {
      case AF_INET:
    *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
    return 1;
      case AF_INET6:
    *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
    return 1;
    }
  }
  return 0;
}
