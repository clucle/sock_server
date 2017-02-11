#pragma once
#include<winsock2.h>
#include<WS2tcpip.h>


#pragma comment(lib,"Ws2_32.lib")

#define PORT "1234"
#define BUFFERSIZE 512




class Client {
    WSADATA wsadata;
    SOCKET ServSocket;

public:
    Client() {
        struct addrinfo *result = NULL, ref;

        WSAStartup(MAKEWORD(2, 2), &wsadata);

        ZeroMemory(&ref, sizeof(ref));
        ref.ai_family = AF_INET;
        ref.ai_protocol = IPPROTO_TCP;
        ref.ai_flags = AI_PASSIVE;
        getaddrinfo("127.0.0.1", PORT, &ref, &result);

        ServSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        connect(ServSocket, result->ai_addr, result->ai_addrlen);
    }

    bool SendMsg(std::string msg) {
        int status = send(ServSocket, msg.c_str(), msg.length(), 0);
        if (status != SOCKET_ERROR) {
            return true;
        }
        else
        {
            shutdown(ServSocket, SD_SEND);
            closesocket(ServSocket);
            WSACleanup();
            return false;
        }
    }

};

