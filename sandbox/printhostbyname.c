/*
Usage: ./a.out

google.com
Internet Address: 142.250.115.139
Internet Address: 142.250.115.101
Internet Address: 142.250.115.138
Internet Address: 142.250.115.113
Internet Address: 142.250.115.102
Internet Address: 142.250.115.100
*/
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

void pr_inet(char **listptr, int length, char *outputstr) {
    struct in_addr *ptr;
    while ( (ptr = (struct in_addr *) *listptr++) != NULL) {
        strcat(outputstr, inet_ntoa(*ptr));
        strcat(outputstr, "\n");
    }
}

int main(int argc, char* argv[]) {
    char outputstr[1024] = "";
    char *ptr;
    struct hostent *hostptr;
    ptr = "google.com";
    hostptr = gethostbyname(ptr);
    if (hostptr == NULL) {
        printf("Error: NULL ptr returned\n");
    } else {
        pr_inet(hostptr->h_addr_list, hostptr->h_length, outputstr);
        printf("Output:\n%s\n", outputstr);
    };
    return 0;
}
