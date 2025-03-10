#include "inet.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in serv_addr;
    pname = argv[0];
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    serv_addr.sin_port        = htons(SERV_TCP_PORT);

    // Open a TCP socket
    printf("Opening internet stream socket\n");
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("client: can't open stream socket\n");
        return -1;
    }
    // Connect to the server
    printf("Opening connection %s:%d\n", SERV_HOST_ADDR, SERV_TCP_PORT);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("client: can't connect to server\n");
        return -1;
    }
    str_cli(stdin, sockfd);
    close(sockfd);
    exit(0);
}