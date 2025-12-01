/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

// the port users will be connecting to
#define PORT "3490"
// how many pending connections queue will hold
#define BACKLOG 10

void sigchld_handler(int s)
{
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    // listen on sock_fd, new connection on new_fd
    int sockfd, new_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int addr_reuse = 1;
    char ip_str[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // bind to the first address we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        struct in_addr *ipv4;
        struct in6_addr *ipv6;

        if (p->ai_family == AF_INET)
        {
            ipv4 = &(((struct sockaddr_in *)p->ai_addr)->sin_addr);
            inet_ntop(p->ai_family, ipv4, ip_str, sizeof ip_str);
        }
        else if (p->ai_family == AF_INET6)
        {
            ipv6 = &(((struct sockaddr_in6 *)p->ai_addr)->sin6_addr);
            inet_ntop(p->ai_family, ipv6, ip_str, sizeof ip_str);
        }
        else
        {
            printf("[-] Unable to determine address family\n");
            exit(1);
        }
        printf("[+] Using address %s\n", ip_str);

        printf("Creating socket.\n");
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("Failed to create socket.\n");
            continue;
        }

        printf("Setting address to be reuseable\n");
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &addr_reuse,
                sizeof(int)) == -1) {
            perror("Failed to set socket to be reuseable.");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("Failed to bind socket.");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("[+] Waiting for connections...\n");
    while(1) {
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr,
                &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            ip_str, sizeof ip_str);
        printf("server: got connection from %s\n", ip_str);

        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            if (send(new_fd, "Hello, world!", 13, 0) == -1)
                perror("send");
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }

    return 0;
}
