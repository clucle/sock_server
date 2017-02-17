#include "client.h"

int main()
{
    Client client;
    client.initWinSock();
    client.runClient();
    return 0;
}