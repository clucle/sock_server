#pragma once
#include<winsock2.h>
#include<WS2tcpip.h>
#include<stdio.h>
#include<Windows.h>
#include<vector>

#pragma comment(lib,"Ws2_32.lib")

#define PORT "1234"
#define BUFFERSIZE 512

class Server {
    WSADATA wsadata;
    SOCKET lSocket;
    DWORD ThreadID;


    int MaxCon = 100;
    
    class ClientInfo {
    public:
        ClientInfo(SOCKET sk, int st, Server* server) {
            Socket = sk; status = st; handle = NULL; instance = server;
        }
        SOCKET Socket;
        int status;
        HANDLE handle;
        Server* instance;
    };
    std::vector<ClientInfo> Clients;

public:
    Server() {
        struct addrinfo *result = NULL, ref;
        wsadata;
        WSAStartup(MAKEWORD(2,2), &wsadata);

        ZeroMemory(&ref, sizeof(ref));
        ref.ai_family = AF_INET;
        ref.ai_socktype = SOCK_STREAM;
        ref.ai_protocol = IPPROTO_TCP;
        ref.ai_flags = AI_PASSIVE;
        getaddrinfo("127.0.0.1", PORT, &ref, &result);

        //listen socket
        lSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        bind(lSocket, result->ai_addr, result->ai_addrlen);
    }

    void UpdateServer() {
        for (size_t i = 0; i < Clients.size(); i++)
        {
            if (Clients[i].status == 0) {
                if (Clients[i].handle == NULL){
                    
                    Clients[i].handle = CreateThread(0, 0, Receive, &Clients[i], 0, &ThreadID);
                }
            }
            else
            {
                Clients.erase(Clients.begin() + i);
            }
        }
    }

    void Listen() {
        do {
            listen(lSocket, SOMAXCONN);

            SOCKET CSocket = INVALID_SOCKET;
            CSocket = accept(lSocket, NULL, NULL);
            if (Clients.size() > MaxCon) continue;
            if (CSocket != INVALID_SOCKET) Clients.push_back(ClientInfo(CSocket, 0, this));
            UpdateServer();
        } while (true);
    }

    void Server::SendToAll(std::string msg) {
        for (size_t i = 0; i < Clients.size(); i++)
        {
            int status = send(Clients[i].Socket, msg.c_str(), msg.length(), 0);
            if (status == SOCKET_ERROR) {
                printf_s("%d clients", Clients.size());
            }
        }
    }

    static DWORD WINAPI Receive(void* client) {
        ClientInfo* CInfo = (ClientInfo*)client;

        int status;
        char revBuffer[BUFFERSIZE];
        do
        {
            memset(&revBuffer[0], 0, sizeof(revBuffer));
            status = recv(CInfo->Socket, revBuffer, BUFFERSIZE, 0);
            std::string msg = revBuffer;
            if (status > 0) {
                printf_s(revBuffer);
                printf_s("\n");
                CInfo->instance->SendToAll(msg);
            }
            else
            {
                break;
            }
        } while (true);
        printf_s("user disconnect\n");
        shutdown(CInfo->Socket, SD_SEND);
        closesocket(CInfo->Socket);
        CInfo->status = 0;

        return 0;
    }
    ~Server() {
        for each (ClientInfo info in Clients)
        {
            CloseHandle(info.handle);
        }

        WSACleanup();
    }
};