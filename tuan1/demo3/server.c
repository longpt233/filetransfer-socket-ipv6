#include <sys/socket.h>    
#include <netinet/in.h> //struct sockaddr_in


#define BUFF_SIZE 256



int main(){
// constuct
    int server;
    server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

// bind 
    struct sockaddr_in serverAddr;
    // struct sockadd_in gom 3 thuco tinh sau 
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5500);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    /*int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);*/
    if(bind(server,(struct sockaddr *)&serverAddr, sizeof(serverAddr))){
        printf("Error! Cannot bind this address.");
        return 0;
    }

// comunicate 
    struct sockaddr_in clientAddr;
    char buff[BUFF_SIZE];
    int ret ;
    while(1){
        //Receive message
        /*ssize_t recvfrom(int socket, void *restrict buffer, size_t length,int flags, struct sockaddr *restrict address,socklen_t *restrict address_len);*/
        ret = recvfrom(server, buff, BUFF_SIZE, 0,(struct sockaddr *) &clientAddr,sizeof(clientAddr));  // tai sao lenght lai lai &??
        if(ret == -1)
            printf("Error ");
        // dung tra ve
        else {
            buff[ret] = "\0";
            printf("Receive from client[%s:%d] %s\n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port),buff);
            //Echo to client
            ret = sendto(server, buff, ret, 0,(struct sockaddr *) &clientAddr,sizeof(clientAddr));
            if(ret == -1)
            printf("Error");
        }
    } 
// close 
    close(server);
    return 0;
}