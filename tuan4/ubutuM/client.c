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

#include <sys/stat.h>   // stat 

void connected(int sockfd,FILE *fp);
long long  read_serv_fsize(int clifd );


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
    if (inet_pton(AF_INET6, "2001:ee0:438f:6c60::5", &serveraddr.sin6_addr) < 0){  //6
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

    FILE *fp = fopen("/home/long/Documents/20192/OOP/document/lap-trinh-java-tonghop.zip", "r+");
    if (fp == NULL) {
        perror("Can't open file");
        exit(1);
    }

    connected(sockfd,fp);

    printf("send successfully");

    fclose(fp);
    close(sockfd);

    return 0;
}

void connected(int sockfd,FILE *fp){ 

    /* receive the file offset to the server */
    long long  serv_fsize=read_serv_fsize(sockfd);
    if(serv_fsize<0){
        perror("read_fsize return <0");
        exit(1);
    }

    fseek(fp ,0L,SEEK_END);
    long long  fsize = (long long )ftell(fp);
    printf("client_fsize=%lld\n",fsize);

    fseek(fp, serv_fsize,SEEK_SET);

    /* Read the data from the socket and write to the file */
    char fbuf[MAX_LINE];
    long long total_send=0;
    while(serv_fsize<fsize) { 
        int byte_read = fread(fbuf, sizeof(char), MAX_LINE, fp);

        if (byte_read <= 0){
            perror("fread file failue");
            exit(1);
        }
        serv_fsize = serv_fsize + byte_read; 
        int byte_send = send(sockfd, fbuf, byte_read,0);
        total_send+=byte_send;
        if (byte_send <= 0){
            perror("send file to socket failue");
        }else{
            printf("\rserv_fsize=%lld   total_send=%lld\n ",serv_fsize,total_send);
        }
    }
}

long long  read_serv_fsize(int clifd){ 
    long long temp_serv_fsize=-1;
    if(read(clifd, (void*) &temp_serv_fsize, sizeof(temp_serv_fsize))<0){
        perror("rev_size_from_server fail\n");
    }else
        printf("serv_size=%lld\n",temp_serv_fsize);
    return temp_serv_fsize;
}

