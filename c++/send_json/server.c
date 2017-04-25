#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#ifdef __WIN32__
# include <winsock2.h>
#else
# include <sys/socket.h>
# include <arpa/inet.h>
#endif

#include <json/json.h>
# define MAX_SIZE 1024

int main()
{
    /* all previous code until
    printf("Reading from client\n"); */

    ssize_t r;

    char buff[MAX_SIZE];
    int connfd = 0;

    for (;;)
    {
        r = read(connfd, buff, MAX_SIZE);

        if (r == -1)
        {
            perror("read");
            return EXIT_FAILURE;
        }
        if (r == 0)
            break;

        printf("READ: %s\n", buff);
    }

    return EXIT_SUCCESS;
}
