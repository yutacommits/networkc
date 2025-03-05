/*

Notes:

       Given node and service, which identify an Internet host and a
       service, getaddrinfo() returns one or more addrinfo structures,
       each of which contains an Internet address that can be specified
       in a call to bind(2) or connect(2).  The getaddrinfo() function
       combines the functionality provided by the gethostbyname(3) and
       getservbyname(3) functions into a single interface, but unlike the
       latter functions, getaddrinfo() is reentrant and allows programs
       to eliminate IPv4-versus-IPv6 dependencies.
*/

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

struct sockaddr_in tcp_srv_addr;
struct servent tcp_serv_info;
struct hostent tcp_host_info;

int tcp_main(char *host, char *service, int port) {
    // int fd, resvport;
    // unsigned long inaddr;
    // char *host_err_str();
    struct servent *sp;
    struct hostent *hp;

    struct addrinfo hints;
    struct addrinfo *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    int s = getaddrinfo(NULL, service, &hints, &result);
    if (s != 0) {
        printf("getaddrinfo() failed\n");
        return -1;
    }
    printf("%s\n", result->ai_protocol);
    // struct addrinfo *result, *rp;
    // struct sockaddr_storage peer_addr;
    return 0;
}


int main() {
    tcp_main("google.com", "ssh", 0);

    return 0;
}
