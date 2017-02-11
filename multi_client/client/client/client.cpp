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

int main(){
    
    std::thread sendThread (sendHandler);

    sendThread.join();

    std::cout << "end thread" << std::endl;

    return 0;
}


