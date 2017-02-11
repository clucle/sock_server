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

    } while (status == true);
}

void recvHandler()
{
    bool status;
    do
    {
        status = client.Receive();
    } while (status == true);
}

int main(){
    
    std::thread sendThread (sendHandler);

    std::thread recvThread (recvHandler);

    sendThread.join();

    recvThread.join();

    std::cout << "end thread" << std::endl;

    return 0;
}


