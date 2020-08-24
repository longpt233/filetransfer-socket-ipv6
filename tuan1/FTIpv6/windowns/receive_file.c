#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <sys/socket.h>
#include<w32api.h>
#include<winsock2.h>
#include<ws2tcpip.h>
//#include<winsock.h>

//#pragma comment(lib, "ws2_32.lib")

#define MAX_LINE 4096
#define LINSTENPORT 7788
#define SERVERPORT 8821
#define BUFFSIZE 4096

void writefile(int sockfd, FILE *fp);
void revtext(int connfd);
int inet_pton(int af, const char *src, void *dst);
int main() {

     WSADATA wsa;
    WORD wVer=MAKEWORD(2,2);
    if(WSAStartup(wVer,&wsa)) 
        printf("version not suppoted");


    SOCKET sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        perror("Can't allocate sockfd");
        exit(1);
    }
    
    struct sockaddr_in6 clientaddr, serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;
    //serveraddr.sin6_addr = in6addr_any;
    serveraddr.sin6_port = htons(SERVERPORT);
     if (inet_pton(AF_INET6, "2001:ee0:43b8:2cf0::3", &serveraddr.sin6_addr) < 0){  // convert to 6: AF_INET6
        perror("IPaddress Convert Error");
         exit(1);
     }  // windown k co ham nay 

    if (bind(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) == -1) {
        perror("Bind Error");
        exit(1);
    }else{
        printf("bind ok\n");
    }

    if (listen(sockfd, LINSTENPORT) == -1) {
        perror("Listen Error");
        exit(1);
    }else{
        printf("listening.....\n");
    }

    socklen_t addrlen = sizeof(clientaddr);
    int connfd = accept(sockfd, (struct sockaddr *) &clientaddr, &addrlen);
    if (connfd == -1) {
        perror("Connect Error");
        exit(1);
    }else{
        printf("connected\n");
    }
    close(sockfd); 

    revtext(connfd);

    FILE *fp = fopen("E:\\testv4\\rev.txt", "wb");
    if (fp == NULL) {
        perror("Can't open file");
        exit(1);
    }
     printf("Start receive");
    char addr[INET6_ADDRSTRLEN];
  //  printf("Start receive file from %s\n", inet_ntop(AF_INET, &clientaddr.sin6_addr, addr, INET6_ADDRSTRLEN));
    writefile(connfd, fp);
    fclose(fp);
    
    closesocket(connfd);
    WSACleanup();
    return 0;
}


void revtext(int connfd){
     char text[BUFFSIZE] = {0}; 
    if (recv(connfd,text, BUFFSIZE, 0) == -1)  {
        perror("Can't receive filename");
        exit(1);
    }else{
        printf("receive : %s\n",text);
    }
};

void writefile(int sockfd, FILE *fp){
    ssize_t n;
    char buff[MAX_LINE] = {0};
    
    ssize_t total=0;
    while ((n = recv(sockfd, buff, MAX_LINE, 0)) > 0) 
    {
	    total+=n;
        if (n == -1)
        {
            perror("Receive File Error");
            exit(1);
        }
        
        if (fwrite(buff, sizeof(char), n, fp) != n)
        {
            perror("Write File Error");
            exit(1);
        }
        memset(buff, 0, MAX_LINE);
    }
    printf("Receive Success, NumBytes = %ld\n", total);
}
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
