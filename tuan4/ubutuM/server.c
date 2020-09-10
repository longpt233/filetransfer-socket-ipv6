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
#include <sys/types.h>


int  send_fsize(int sockfd,long long  fsize_received);
void loop_accept(int sockfd ,FILE* fp);
int conn(int sockfd);

int main() {

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
    if (inet_pton(AF_INET6, "::1", &serveraddr.sin6_addr) < 0){
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
        printf("init queue....\n");
    }

    FILE* file=fopen("/home/long/Documents/20193/Project1/tuan4/ubutuM/mesreceived.txt", "r+");
    if(file== NULL) {
        perror("open file error");
        exit(1);
    }
    loop_accept(sockfd,file);

    close(sockfd);
    return 0;
}

/*return sock connect from listen sock */
int conn(int sockfd){
    struct sockaddr_in6 cliaddr;
    socklen_t addr_len = sizeof(cliaddr);
    int connfd =accept(sockfd, (struct sockaddr*) &cliaddr, &addr_len);
    if (connfd  < 0) {
        printf("err when accept connect");
    }else { 
        char addr[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &cliaddr.sin6_addr, addr, INET6_ADDRSTRLEN);
        printf("conneted from %s\n",addr); 
    } 

    return connfd;
}

void loop_accept(int sockfd ,FILE* fp){
    while (1){
        printf("listening....\n"); 
        /*connect */
        int connfd=conn(sockfd);
        /*send size*/
        fseek(fp ,0L,SEEK_END);
        printf("serv_fsize_before_recv=%lld\n",(long long )ftell(fp));
        send_fsize(connfd,(long long )ftell(fp));
        /* receive */
        char buff_recv[MAX_LINE];
        long long fsize_received=0;

        // LINUX
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        if(setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv)<0){
            perror("setsockopt failue\n");
        }


        while(1) { 
            int recv_byte = recv(connfd,buff_recv, MAX_LINE, MSG_WAITALL); 
            if (recv_byte < 0){
                perror("recv err\n");
                break;
            }
            else if (recv_byte < MAX_LINE){
                printf("recv byte= %d < MAXLINE",recv_byte);
                break;
            }
            int fwrite_status = fwrite( buff_recv, sizeof(char), MAX_LINE, fp);

            if (fwrite_status < 0){
                perror("fwrite to file err");
                break;
            }
            fsize_received += fwrite_status;
            printf("\r fsize_recv =%lld \n",fsize_received);  
        
        }

        printf("\rnum receive in session =%lld \n",fsize_received);
        fseek(fp ,0L,SEEK_END);
        printf("serv_fsize_after_recv=%lld\n",(long long )ftell(fp)); 
        if (connfd >= 0)
            close(connfd);
        printf("end session....!\n");
    }
    
}

/* Write the file offset to the client */ 
int send_fsize(int connfd,long long  fsize_received){ 
    int status = write(connfd, (void*) &fsize_received , sizeof(fsize_received));

    if (status<0 ){
        printf("wrong when send off_write\n");
        return -1;
    }else {
        printf("send serv_fsize=%lld\n",fsize_received);
        return 1;
    }
    
}


