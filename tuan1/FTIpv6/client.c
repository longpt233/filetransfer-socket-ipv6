// Client on ubutu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// socket
#include <unistd.h>  // close(sockfd)
#include <sys/socket.h>   // socket
// constance
#include <arpa/inet.h>  // inet_pton()
#include <netinet/in.h>   // struct sockaddr_in

#include "transfer.h"

void sendfile(FILE *fp, int sockfd);
void sendtext(int sockfd); 

int main(){
    /**init socket*/
    // fd= file decoration ?
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);// remove 6
    if (sockfd < 0) {
        perror("Can't allocate sockfd");
        exit(1);
    }
    /*init info server*/
    struct sockaddr_in6 serveraddr;      //6
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;     //6
    serveraddr.sin6_port = htons(SERVERPORT);  //6
    //serveraddr.sin6_addr=in6addr_loopback;
    if (inet_pton(AF_INET6, "2001:ee0:43b8:2cf0::3", &serveraddr.sin6_addr) < 0){  //6
        perror("IPaddress Convert Error");
        exit(1);
    }
    /*conn to server*/
    if (connect(sockfd, ( struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0){
        perror("Connect Error");
        exit(1);
    }else {
        printf("connected!\n");
    }
    /**/
    sendtext(sockfd);   // send text

    FILE *fp = fopen("/home/long/Documents/20193/Project1/tuan1/FTIpv6/mes.txt", "rb");
    if (fp == NULL) {
        perror("Can't open file");
        exit(1);
    }

    sendfile(fp, sockfd);  // sent file by *fp 
    fclose(fp);
    
    close(sockfd);
    return 0;
}

void sendfile(FILE *fp, int sockfd) {
    int n; 
    char sendline[MAX_LINE] = {0}; 
    ssize_t total=0;  // data structure count byte sent
    while ((n = fread(sendline, sizeof(char), MAX_LINE, fp)) > 0) {
	    total+=n;
        if (n != MAX_LINE && ferror(fp)){
            perror("Read File Error");
            exit(1);
        }
        
        if (send(sockfd, sendline, n, 0) == -1){
            perror("Can't send file");
            exit(1);
        }else{
            printf("#");  // progess
        }
        memset(sendline, 0, MAX_LINE);
    }
    printf("\n");
    printf("Send Success, NumBytes = %ld\n", total);
}

void sendtext(int sockfd){ 
    char *text = "hello server !";
    
    char buff[BUFFSIZE] = {0};
    strncpy(buff, text, strlen(text));
    if (send(sockfd, buff, BUFFSIZE, 0) == -1){
        perror("Can't send ");
        exit(1);
    }else{
        printf("sent text !\n");
    }
}
