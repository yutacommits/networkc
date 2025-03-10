#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SERV_UDP_PORT 6543
#define SERV_TCP_PORT 6543
#define SERV_HOST_ADDR "127.0.0.1"

char *pname;
