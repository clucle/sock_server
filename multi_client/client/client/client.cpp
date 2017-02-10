#include<winsock2.h>
#include<WS2tcpip.h>
#include<stdio.h>
#include<iostream>
#include<string>

#pragma comment(lib,"Ws2_32.lib")

#define PORT "1234"
#define BUFFERSIZE 512


struct addrinfo *result = NULL, ref;

int main(){
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);

    ZeroMemory(&ref, sizeof(ref));
    ref.ai_family = AF_INET;
    ref.ai_protocol = IPPROTO_TCP;
    ref.ai_flags = AI_PASSIVE;
    getaddrinfo("127.0.0.1", PORT, &ref, &result);

    SOCKET Ani = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    connect(Ani, result->ai_addr, result->ai_addrlen);
    int status;
    std::string sendMsg;
    do
    {
        std::getline(std::cin, sendMsg);
        status = send(Ani, sendMsg.c_str(), sendMsg.length(), 0);

    } while (status!=SOCKET_ERROR);

    shutdown(Ani, SD_SEND);
    closesocket(Ani);
    WSACleanup();
    return 0;
}


