/*
Read a datagram from a connectionless socket and write it back to the sender.

We never return, as we never know when a datagram client is done.
*/
#include "inet.h"
#include "utils.h"

void dg_echo(int sockfd, struct sockaddr *pcli_addr, int maxclilen) {
    /*
        sockfd: socket file descriptor
        *pcli_addr: ptr to appropriate sockaddr_XX strucutre
        maxclilen: sizeof(*pcli_addr)
    */
    int n, clilen;
    char mesg[MAXMESG];

    for (;;) {
        clilen = maxclilen;
        memset(&mesg, 0, sizeof(mesg));
        n = recvfrom(sockfd, mesg, MAXMESG, 0, pcli_addr, &clilen);
        printf("~#");
        if (n < 0)
            perror("dg_echo: recvfrom error");
        // if (sendto(sockfd, mesg, n, 0, pcli_addr, clilen) != n) 
        //     perror("dg_echo: sendto err");
        // else
        printf("%s\n", mesg);
    }
}
