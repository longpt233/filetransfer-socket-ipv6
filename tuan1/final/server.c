/* 1.Tạo các #include cần thiết */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
/* dành riêng cho AF_INET */
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFF 256

int main(){
    int server_sockfd;
    int  client_len;
    /* 1. Thực hiện khởi tạo socket mới cho trình chủ */
    server_sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    
    /*  Đặt tên và gán địa chỉ kết nối cho socket theo giao thức Internet */
    struct sockaddr_in server_address;
    //memset(&server_addr, '0', sizeof(server_addr));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    server_address.sin_port = htons( 15004 );
   
    /* 1. Ràng buộc tên với socket */
    bind( server_sockfd, (struct sockaddr *)&server_address, sizeof( server_address ) );
    
    /* 2. Mở hàng đợi nhận kết nối - cho phép đặt hàng vào hàng đợi tối đa 5 kết nối */
    if (listen( server_sockfd, 5 )==0) printf("listend !\n");
    
    
    /* 2. chờ và xử lý kết nối của trình khách */
    int client_sockfd;
    struct sockaddr_in client_address;
    /* 2.Chờ và chấp nhận kết nối . có thể lặp từ bước này */
    client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_len );
    if (client_sockfd >=0) printf("accepted !\n");
    
    //Chua thong tin dia chi client ket noi den
    char s[256];
    inet_ntop(client_address.sin_family,(struct sockaddr *)&client_address,  s, sizeof s);
    printf("server: got connection from %s\n", s);

    // communicate
    char buff[MAX_BUFF];
    
        printf( "server waiting...\n" );
        /* Đọc dữ liệu do trình khách gửi đ ến */
        read( client_sockfd, &buff, MAX_BUFF);
        printf("client say: %s \n",buff);
        printf("server say : ");
        gets(buff);
        fflush(stdin);
        //  chep xau : strcpy(buff, "+Server has got message\n");
        strcat(buff, "Server has got message\n");
        /* Gửi trả dữ liệu về cho trình khách */
        write(client_sockfd, &buff ,MAX_BUFF);
    
    
    /*4. Đóng kết nối */
        close( client_sockfd );
        close(server_sockfd);
        exit(0);
}