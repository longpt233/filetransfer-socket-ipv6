#include <sys/socket.h>    //Thư viện chứa các cấu trúc cần thiết cho socket
#include <netinet/in.h>
#include<stdio.h> // gets_s?


#define BUFF_SIZE 256

int main(){
// Construct socket
    int client;
    client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
// Specify server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5500);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//: Communicate with server
    char buff[BUFF_SIZE];
    int ret, serverAddrLen = sizeof(serverAddr);
    do {
        //Send message
        printf("Send to server: ");
        //gets_s(buff, BUFF_SIZE); // loi . doc vao buff k qua BUFF_SIZE
        fgets(buff,BUFF_SIZE,stdin);
        /*ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);*/
        ret = sendto(client, buff, strlen(buff), 0,(struct sockaddr *) &serverAddr, serverAddrLen);
        if(ret == -1)
            printf("Error! Cannot send mesage.");
        // true :return the number of characters sent
        //Receive echo message
        ret = recvfrom(client, buff, BUFF_SIZE, 0,(struct sockaddr *) &serverAddr,&serverAddrLen);
        if(ret == -1)
            printf("error");
        else {
            buff[ret] = "\0";
            printf("Receive from client[%s:%d] %s\n",inet_ntoa(serverAddr.sin_addr),ntohs(serverAddr.sin_port),buff);
        }
        fgets(buff,BUFF_SIZE,stdin);
    }while(strcmp(buff, "BYE") != 0); //end while
// Close socket
    close(client);


    return 0;

}