// server on ubutu 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "transfer.h"



#include <sys/stat.h>   // stat
#include <fcntl.h>      //open
#include <sys/types.h>


void send_fsize(int sockfd,uint32_t fsize_received);
void wait_conn(int sockfd ,int fp);

int main() 
{

   /*Init Socket*/
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Can't allocate sockfd");
        exit(1);
    }
    
    /*infor*/
    struct sockaddr_in6  serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;
    if (inet_pton(AF_INET6, "2405:4800:170e:f10b:c14b:e5fd:13d2:588a", &serveraddr.sin6_addr) < 0){
        perror("IPaddress Convert Error");
        exit(1);
    }
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

    int file=open("/home/long/Documents/20193/Project1/tuan3/ubutuM/mesreceived.txt", O_RDWR | O_CREAT,0644);
    wait_conn(sockfd,file);

    close(sockfd);
    return 0;
}

void wait_conn(int sockfd ,int fp){
    while (1){
        struct sockaddr_in6 cliaddr;
        socklen_t addr_len = sizeof(cliaddr);
        int connfd =accept(sockfd, (struct sockaddr*) &cliaddr, &addr_len);
        if (connfd  < 0) {
            printf("err when accept connect");
            goto next_cli;
        
        }else { 
            char addr[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &cliaddr.sin6_addr, addr, INET6_ADDRSTRLEN);
            printf("conneted from %s\n",addr); 
        } 

        struct stat statbuf;
        fstat(fp, &statbuf); 
        uint32_t fsize_received = statbuf.st_size;
        send_fsize(connfd,fsize_received);

        /* receive */

        char buff_recv[MAX_LINE];

        while(1) { 
            int rd_return = read(connfd, buff_recv, sizeof(buff_recv)); 
            if (rd_return < 0)
                exit(1);
            else if (rd_return == 0)
                break;

            int wr_status = 0;

            for ( ; ; ) {
                int wr_return = pwrite(fp, buff_recv + wr_status, rd_return - wr_status, fsize_received);

                if (wr_return < 0)
                    goto next_cli;

                wr_status = wr_status + wr_return;
                fsize_received = fsize_received + wr_return;
                printf(" num receive=%ld num write= %ld\n",fsize_received,wr_status);  

                if (wr_status == rd_return)
                    break;
            }
        }

        next_cli:

        if (connfd >= 0)
            close(connfd);
    }
    
}

void send_fsize(int connfd,uint32_t fsize_received){
  /* Write the file offset to the client */ 
    uint32_t temp_fsize_received = htonl(fsize_received);
  //  printf("vao 130");

    int off_write = 0;
    while (1) {
        int wr_return = write(connfd, (void*) &temp_fsize_received + off_write, sizeof(temp_fsize_received) - off_write);

        if (wr_return <= 0){
            printf("wrong when sned off_write");
        }
        off_write = off_write + wr_return;

        if (off_write == sizeof(temp_fsize_received)){
            printf("send off write successfully , fsize_received=%ld",temp_fsize_received);
            return;
        }
    }
}


