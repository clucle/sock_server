#include<stdio.h>
#include<iostream>
#include<string>

#include "client.h"


int main(){
    Client cli;

    std::string sendMsg;
    do
    {
        std::getline(std::cin, sendMsg);
        cli.SendMsg(sendMsg);

    } while (true);

    return 0;
}


