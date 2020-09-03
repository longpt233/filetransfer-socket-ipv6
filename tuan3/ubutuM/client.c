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
#include <fcntl.h>      //open

void connected(int sockfd,FILE *fp);
int read_fsize(int clifd,uint32_t temp_serv_fsize);


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
    if (inet_pton(AF_INET6, "2405:4800:170e:f10b:c14b:e5fd:13d2:588a", &serveraddr.sin6_addr) < 0){  //6
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

    FILE *fp = fopen("/home/long/Documents/20193/Project1/tuan1/NAT.pdf", "r+");
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
    uint32_t temp_size = 0;
    if(read_fsize(sockfd,temp_size)==-1) {
        perror("read_fsize falue");
    }
    uint32_t serv_fsize = ntohl(temp_size);

    struct stat statbuf;
    if (fstat(fileno(fp), &statbuf) != 0)
        perror("fstat err");
    uint32_t fsize = statbuf.st_size;
    printf("serv_fsize=%ld\n",fsize);

    /* Read the data from the socket and write to the file */
    char fbuf[MAX_LINE];

    while(serv_fsize<fsize) { 
        int rd_return = pread(fileno(fp), fbuf, sizeof(fbuf), serv_fsize);

        if (rd_return <= 0){
            perror("pread file failue");
            exit(1);
        }
        
        serv_fsize = serv_fsize + rd_return;

        int wr_status = 0;

        printf("serv_fsize_return=%ld\n",serv_fsize);

        while (1) {
            int wr_return = write(sockfd, fbuf + wr_status, rd_return - wr_status);

            if (wr_return <= 0){
                perror("write file to socket failue");
            }

            wr_status = wr_status + wr_return;

            if (wr_status == rd_return)
                break;
        }
    }

    
    
}

int read_fsize(int clifd,uint32_t temp_serv_fsize){ 
    int off_read = 0;

    while(1) {
        int rd_status = read(clifd, (void*) &temp_serv_fsize, sizeof(temp_serv_fsize) - off_read);

        if (rd_status == -1)
            return -1;
        else if (rd_status == 0 && off_read != sizeof(temp_serv_fsize))
            return -1;

        off_read = off_read + rd_status;

        if (off_read == sizeof(temp_serv_fsize))
            return 1;
    }

}

