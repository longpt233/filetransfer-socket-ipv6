#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main()
{
    /* Initialize the file */

    int fd = open("download", O_RDWR | O_CREAT,0644);

    struct stat statbuf;
    fstat(fd, &statbuf);

    uint32_t cli_fsize = statbuf.st_size;

    /* Initialize the connection with the server */

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    servaddr.sin_port = htons(5556);

    if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) != 0) {
        exit(1);
    }

    /* Write the file offset to the server */

    uint32_t n_cli_fsize = htonl(cli_fsize);

    int off_write = 0;

    for ( ; ; ) {
        int wr_return = write(sockfd, (void*) &n_cli_fsize + off_write, sizeof(n_cli_fsize) - off_write);

        if (wr_return <= 0)
            exit(1);

        off_write = off_write + wr_return;

        if (off_write == sizeof(n_cli_fsize))
            break;
    }

    /* Read the data from the socket and write to the file */

    char fbuf[100];

    for ( ; ; ) {
        int rd_return = read(sockfd, fbuf, sizeof(fbuf));
         printf("receive sussest%ld\n",rd_return);
        if (rd_return < 0)
            exit(1);
        else if (rd_return == 0)
            break;

        int wr_status = 0;

        for ( ; ; ) {
            printf("cli_fsize=%ld",cli_fsize);
            int wr_return = pwrite(fd, fbuf + wr_status, rd_return - wr_status, cli_fsize);
            printf("write sussest%ld\n",wr_return);
            if (wr_return < 0)
                perror("lo");
            
            wr_status = wr_status + wr_return;
            cli_fsize = cli_fsize + wr_return;

            if (wr_status == rd_return)
                break;
        }
    }

    return 0;
}