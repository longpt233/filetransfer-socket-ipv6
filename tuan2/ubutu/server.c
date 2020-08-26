// server on ubutu 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "transfer.h"

void writefile(int sockfd, FILE *fp);
void revtext(int connfd);
int main() 
{

   /*Init Socket*/
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Can't allocate sockfd");
        exit(1);
    }
    
    /*infor*/
    struct sockaddr_in6 clientaddr, serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;
    //serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //serveraddr.sin6_addr =in6addr_any;
    if (inet_pton(AF_INET6, "2402:800:61c7:5445:68c4:725a:6fd4:83a6", &serveraddr.sin6_addr) < 0){
        perror("IPaddress Convert Error");
        exit(1);
    }
    //serveraddr.sin_addr.s_addr =inet_addr("");
    serveraddr.sin6_port = htons(SERVERPORT);

    /**/
    if (bind(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) == -1) {
        perror("Bind Error");
        exit(1);
    }else{
        printf("bind done !\n");
    }
    /**/
    if (listen(sockfd, MAX_QUEUE) == -1) {
        perror("Listen Error");
        exit(1);
    }else{
        printf("listening....\n");
    }

    /**/
    socklen_t addrlen = sizeof(clientaddr);
    int connfd = accept(sockfd, (struct sockaddr *) &clientaddr, &addrlen);
    if (connfd == -1) {
        perror("Connect Error");
        exit(1);
    }else{
        char addr[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &clientaddr.sin6_addr, addr, INET6_ADDRSTRLEN);
        printf("conneted from %s\n",addr);
    }
    close(sockfd); // sao lai phai close tai day ?

    /**/
    revtext(connfd);

    FILE *fp = fopen("/home/long/Documents/20193/Project1/tuan1/FTIpv6/mesreceived.txt", "wb");
    if (fp == NULL) {
        perror("Can't open file");
        exit(1);
    }
    printf("Start receive...\n");
    writefile(connfd, fp);
    

    fclose(fp);
    close(connfd);
    return 0;
}

void writefile(int sockfd, FILE *fp){
    ssize_t n;
    ssize_t total=0;
    char buff[MAX_LINE] = {0};
    // BLOCKING
    while ((n = recv(sockfd, buff, MAX_LINE, MSG_WAITALL)) > 0)  {
	    total+=n;
        if (n == -1){
            perror("Receive File Error");
            exit(1);
        }
        
        if (fwrite(buff, sizeof(char), n, fp) != n){
            perror("Write File Error");
            exit(1);
        }
        memset(buff, 0, MAX_LINE);
    }

    printf("Receive Success, NumBytes = %ld\n", total);
}


void revtext(int connfd){
    char text[BUFFSIZE] = {0}; 
    if (recv(connfd, text, BUFFSIZE, 0) == -1) {
        perror("Can't receive mes");
        exit(1);
    }else{
        printf("mes received: %s\n",text);
    }
}