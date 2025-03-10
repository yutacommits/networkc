/*
Example of server using TCP protocol.

reference: pg.284 TCP Example
*/
// #include <arpa/inet.h>
#include "inet.h"
#include "utils.h"

extern char* pname;

int main(int argc, char *argv[]) {
    int                 sockfd, newsockfd, clilen, childpid;
    struct sockaddr_in  cli_addr, serv_addr;
    pname = argv[0];

    // NOTE: the book uses err_dump, but this is defined
    // within a utility in misc. section of book not within
    // the section 6.6
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror("server: can't open stream socket");

    // bind local address, so that the client can send to us
    // bzero((char *) &serv_addr, sizeof(serv_addr));
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(SERV_TCP_PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("server: can't bind local address");
        return -1;
    }
    listen(sockfd, 5);

    for (;;) {
        /*
        Wait for connection from a client process.
        This is an example of a concurrent server.
        */
        clilen = sizeof(cli_addr);
        printf("Starting to accept connections,...\n");
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        printf("Incoming connection,..\n");
        if (newsockfd < 0)
            perror("server: accept error");
        if (( childpid = fork()) < 0)
            perror("server: form error");
        else if (childpid == 0) {
            printf("closing out sockfd.\n");
            close(sockfd);
            printf("sockfd closed\n");
            printf("str_echo(newsockfd)\n");
            str_echo(newsockfd);
            printf("exiting\n");
            exit(0);
        }
        printf("Closing socket descriptor.\n");
        close(newsockfd);
    }

}
