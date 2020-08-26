## Task lists
- [x] transfer simple pdf  from ubutu to ubutu 
- [x] zip (up to 400mb) windows to ubutu 
- [x] send text to server

## Hello to markdown
- Ctrl + shift + v: view 
- Ctrl + shift + i: format table 

## Content 


1. intro 
 
 | Server        | Client  |
 | ------------- | ------- |
 | init socket   |         |
 | init sockaddr |
 | bind          | x       |
 | listen        | x       |
 | accept        | connect |
 | read/write    |
 | close         |

2. init socket 
+ include<sys/socket.h>
+ int sockfd = socket(int domain , int type , int protocol);
+ return a file descriptor (by int value )
+ domain = AF_INET6, AF_INET,AF_UNIX,AF_ISO,...
+ type = SOCK_STREAM, SOCK_DGRAM, SOCK_SEQPACKET
+ protocol= usually is 0 (default)
  
3. struct sockaddr
+ include<netinet/in.h>
+ client has only one sockaddr of server and server has both 2 
+ is a struct 
  * sin_family: AF_INET
  * sin_port: htons()
  * sin_addr: is struct in_addr 
    |           | addr-> string | string-> addr |
    | --------- | ------------- | ------------- |
    | both      | inet_ntop     | inet_pton     |
    | only ipv4 | inet_ntoa     | inet_addr     |

4. bind
+ use struct sockaddr server 
  
5. listen
+ use int socket  
  
6. accept
+ take information client which conect

7. conect
+ connect to server which has information in sockaddr server
+ in this step, client and server can read/ write over int sockfd

8. read/write 
+ use only sockfd 

9. close 

## note 
- run : make
- on windows: compiler with   -lws2_32  flag 
- MSG_WAITALL : only on server 
- Bind failed: Address already in use  => change port 
- other error : check th ip addr
