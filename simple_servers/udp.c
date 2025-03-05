/*
Example of server using UDP protocol.

socket()
bind()
recvfrom()

struct sockaddr_in {
    sa_family_t     sin_family;     // AF_INET
    in_port_t       sin_port;       // Port number
    struct in_addr  sin_addr;       // IPv4 address
};

uint32_t htonl(uint32_t hostlong);

Reference: pg.287
*/

#include "inet.h"
#include "utils.h"

char *pname = NULL;

int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in serv_addr, cli_addr;
    pname = argv[0];

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("server: can't open datagram socket");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_UDP_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("server: can't bind local address\n");
        return -1;
    }
    printf("Binded to the loop back address\n");
    dg_echo(sockfd, (struct sockaddr *) &cli_addr, sizeof(cli_addr));
}