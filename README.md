## Task lists
- [x] transfer simple pdf  from ubutu to ubutu 
- [x] zip (up to 400mb) windows to ubutu 
- [x] send text to server
- [ ] reconect ? remember check point FTP 

## Hello to markdown
- Ctrl + shift + v: view 
- Ctrl + shift + i: format table 
-  bold * * 

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
+ read/write (int fs, const void *buf, ssize_t N) *use only sockfd *
+ send/recv : is addition of read/write, with flag 
+ fread/fwrite : (const void * ptr, size_t size, size_t count, FILE * stream )
+ pread/pwrite : (int fd, void *buf, size_t count, off_t offset)
+ sendto/recvfrom: datagram (connectioless)

1. close 

## note 
- run : make
- on windows: compiler with   -lws2_32  flag 
- MSG_WAITALL : only on server 
- r+ : r+w,create if not exist
- fopen (return FILE*) call sys-call: open (return fd)
    + fileno(FILE* ) return integer descriptor 
- stat : return struct fd (int ) of file  
- fseek() , ftell()
- du -h file  && md5sum file.


## Error
- A socket call may fail for any of several reasons, including:
    + lack of memory (ENOBUFS)
    + request for an unknown protocol (EPROTONOSUPPORT)
    + request for a type of socket for which there's no supporting protocol (EPROTOTYPE) 
- Bind failed: Address already in use  => change port 
- other error : check th ip addr
- printf not immediately : \n 