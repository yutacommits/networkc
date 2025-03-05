#include <stdio.h>
#include <errno.h>

int main() {
    errno = EACCES;
    perror("Test error");
    return 0;
}
