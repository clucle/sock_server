#include "server.h"

int main()
{
    Server serve;
    serve.initWinSock();
    serve.runServer();

    return 0;
}