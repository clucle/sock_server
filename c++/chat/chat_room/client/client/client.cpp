#include "client.h"

int main()
{
    Client client;
    if(!client.initWinSock()) return -1;
    client.runClient();
    return 0;
}