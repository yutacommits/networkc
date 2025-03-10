
#include <stdio.h>
#include <unistd.h>

#include <string.h>

// for dg_echo
#include <sys/types.h>
#include <sys/socket.h>
/*
* Read "n" bytes from a descriptor
* Use in place of read9) when fd is a stream socket.
*/

#define MAXMESG 2048
#define MAXLINE 512


int readn(int fd, char *ptr, int nbytes) {
    int nleft, nread;
    nleft = nbytes;
    while (nleft > 0) {
        nread = read(fd, ptr, nleft);
        if (nread < 0)
            return nread;
        else if (nread == 0)
            break;
        nleft -= nread;
        ptr += nread;
    }
    return nbytes - nleft;

}

/*
* Write "n" bytes to a descriptor
* Use in place of write() when fd is a stream socket
*/
int written(int fd, char *ptr, int nbytes) {
    int nleft, nwritten;
    nleft = nbytes;
    while (nleft > 0) {
        nwritten = write(fd, ptr, nleft);
        if (nwritten <= 0)
            return nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    return nbytes - nleft;
}

/*
* Read a line from a descriptor. Read the line one byte at a time,
* looking for the newline. We store the newline in the buffer,
* then follow it with a null (the same as fgets(3)).
* We return the number of characters up to, but not including
* the null (the same as strlen(3))
*/
    int readline(int fd, char *ptr, int maxlen) {
        int n, rc;
        char c;
        for (n = 1; n < maxlen; n++) {
            if ( ( rc = read(fd, &c, 1)) == 1) {
                *ptr++ = c;
                if (c == '\n')
                    break;
            }
            else if (rc == 0) {
                if (n == 1)
                    return 0;
                else
                    break;
            }
            else
                return -1;
        }
        *ptr = 0;
        return n;
    }


/*
* Read a stream socket one line at a time, and write each line
* back to the sender.
*/
void str_echo(int sockfd) {
    int n;
    char line[MAXLINE];
    for (;;) {
        n = readline(sockfd, line, MAXLINE);
        printf("n: %d\n", n);

        if (n == 0) {
            printf("str_echo: connection terminated\n");
            return;
        }
        else if (n < 0) {
            printf("str_echo: readline error\n");
            return;
        }

        printf("Writing %s\n", line);
        if (written(sockfd, line, n) != n) {
            printf("str_echo: written error\n");
            return;
        }
    }
}


/*
* Read the contents of the FILE *fp, write each line to the
* stream socket (to the server process), then read a line back from
* the socket and write it to the standard output
*
* Return to caller when an EOF is encountered on the input file
*/

void str_cli(FILE *fp, int sockfd) {
    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        memset(recvline, 0, sizeof(recvline));

        n = strlen(sendline);
        printf("Attempting to send:\n\t%s\n", sendline);
        if (written(sockfd, sendline, n) != n) {
            printf("str_cli: written error on socket\n");
        }

        n = readline(sockfd, recvline, MAXLINE);
        if (n < 0)
            printf("str_cli: readline error\n");
        else if ( n == 0)
        {
            printf("str_cli: The server closed the connection\n");
        }
        printf("rcved stdout:\n\t");
        fputs(recvline, stdout);
    }

    if (ferror(fp))
        printf("str_cli: error reading file\n");
}

/*
* Read a datagram from a connectionless socket and write it back to
* the sender.
*
* We never return, as we never know when a datagram client is done.
*/
void dg_echo(int sockfd, struct sockaddr *pcli_addr, int maxclilen) {
    int n, clilen;
    char mesg[MAXMESG];
    for (;;) {
        clilen = maxclilen;
        n = recvfrom(sockfd, mesg, MAXMESG, 0, pcli_addr, &clilen);
        if (n < 0)
            printf("dg_echo: recvfrom error\n");
        if (sendto(sockfd, mesg, n, 0, pcli_addr, clilen) != n)
            printf("dg_echo: sendto error\n");
    }

}

/*
* Read the contents of the FILE *fp write each line to the
* datagram socket, then read a line back from the datagram
* socket and write it to the standard output.
*
* Return to caller when an EOF is encountered on the input file.
*/
void dg_cli(FILE *fp, int sockfd, struct sockaddr *pserv_addr, int servlen) {
    int n;
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        n = strlen(sendline);
        if (sendto(sockfd, sendline, n, 0, pserv_addr, servlen) != n)
            printf("dg_cli: sendto error on sockete\n");
        n = recvfrom(sockfd, recvline, MAXLINE, 0, (struct sockaddr *) 0, (int *) 0);
        if (n < 0)
            printf("dg_cli: recvfrom error\n");
        recvline[n] = 0;
        fputs(recvline, stdout);
        if (ferror(fp))
            printf("dg_cli: error reading file\n");
    }

}
