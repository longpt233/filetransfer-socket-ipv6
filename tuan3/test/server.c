#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    /* File initializations */

    FILE* fp = fopen("data.bin", "r+");

    if (fp == NULL)
        exit(1);

    // struct stat statbuf;

    // if (fstat(fileno(fp), &statbuf) != 0)
    //     exit(0);

    // uint32_t fsize = statbuf.st_size;

    /* Initialize the server */

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), clifd, addr_len;

    struct sockaddr_in servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(5556);
    if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) != 0) {
        exit(1);
    }

    listen(sockfd, 5);

    for ( ; ; ) {

        addr_len = sizeof(cliaddr);

        if ((clifd = accept(sockfd, (struct sockaddr*) &cliaddr, &addr_len)) < 0) {
            goto next_cli;
        }

        /* Read the 32 bit offset */

        uint32_t n_cli_fsize = 0;
        int off_read = 0;

        for ( ; ; ) {
            int rd_status = read(clifd, (void*) &n_cli_fsize, sizeof(n_cli_fsize) - off_read);

            if (rd_status == -1)
                goto next_cli;
            else if (rd_status == 0 && off_read != sizeof(n_cli_fsize))
                goto next_cli;

            off_read = off_read + rd_status;

            if (off_read == sizeof(n_cli_fsize))
                break;
        }

        uint32_t cli_fsize = ntohl(n_cli_fsize);

        /* Read from the file and write to the socket */

        char fbuf[100];

        for ( ; ; ) {
            int rd_return = pread(fileno(fp), fbuf, sizeof(fbuf), cli_fsize);

            if (rd_return <= 0)
                goto next_cli;

            cli_fsize = cli_fsize + rd_return;

            int wr_status = 0;

            for ( ; ; ) {
                int wr_return = write(clifd, fbuf + wr_status, rd_return - wr_status);

                if (wr_return <= 0)
                    goto next_cli;

                wr_status = wr_status + wr_return;

                if (wr_status == rd_return)
                    break;
            }
        }

        next_cli:

        if (clifd >= 0)
            close(clifd);
    }

    return 0;
}