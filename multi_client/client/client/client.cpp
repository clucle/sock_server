#include<stdio.h>
#include<iostream>
#include<string>
#include<thread>

#include "client.h"

Client client;

void sendHandler()
{
    bool status;

    std::string sendMsg;
    do
    {
        std::getline(std::cin, sendMsg);
        status = client.SendMsg(sendMsg);
        if (status == false) break;
    } while (true);
}

void recvHandler()
{
    bool status;
    do
    {
        status = client.Receive();
        if (status == false) break;
    } while (true);
}

int main(){
    
    std::thread sendThread (sendHandler);

    std::thread recvThread (recvHandler);

    sendThread.join();

    recvThread.join();

    std::cout << "end thread" << std::endl;

    return 0;
}


