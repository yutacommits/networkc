#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

void pr_inet(char **listptr, int length) {
    struct in_addr *ptr;
    while ( (ptr = (struct in_addr *) *listptr++) != NULL)
        printf("Internet Address: %s\n", inet_ntoa(*ptr));
}

int main(int argc, char* argv[]) {
    char *ptr;
    struct hostent *hostptr;
    ptr = argv[1];
    printf("%s\n", ptr);
    hostptr = gethostbyname(ptr);
    if (hostptr == NULL) {
        printf("Error: NULL ptr returned\n");
    } else {
        pr_inet(hostptr->h_addr_list, hostptr->h_length);
    };
    return 0;
}
