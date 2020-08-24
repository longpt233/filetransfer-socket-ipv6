/* 1. Tạo các #include cần thiết để gọi hàm socket */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
/* dành riêng cho AF_INET */
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFF 256

int main(){
    /* số mô tả socket – socket handle */
    int sockfd;
    /* 1. Tạo socket cho trình khách. Lưu lại số mô tả socket */
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    
    /* structure sockaddr_in , chứa các thông tin về socket AF_INET */
    struct sockaddr_in address;
    /*  Đặt tên và gán địa chỉ kết nối cho socket theo giao thức Internet */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    address.sin_port = htons( 15004 );
    
    
    /* 2. Thực hiện kết nối */
    if ( connect( sockfd, (struct sockaddr*)&address, sizeof(address) ) == -1 ) {
        perror( "Oops: client1 problem" );
        exit( 1 );
    }

    char buff[MAX_BUFF];   //='0' ;
    memset(buff, 0, MAX_BUFF);
    

        /* 3. Sau khi socket kết nối, chúng ta có thể đọc ghi dữ liệu của socket tương tự đọc ghi trên file */
        printf("client say : ");
        //fgets(buff,255,stdin); //Chua thong diep doc tu ban phim 
        gets(buff);
        fflush(stdin);
        write( sockfd, &buff, MAX_BUFF );
        read ( sockfd, &buff, MAX_BUFF );
        // buff recv != sent nhung ow day dung 1 cai cho tien
        printf( "char from server: %s\n", buff );
    
    /*4. Đóng kết nối */    
        close( sockfd );
        exit( 0 );
}