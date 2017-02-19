#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")

#define IP_ADDRESS "127.0.0.1"
#define DEFAULT_PORT "3504"
#define DEFAULT_BUFLEN 512

struct client_type
{
    SOCKET socket;
    int id;
    char received_message[DEFAULT_BUFLEN];
    std::string name;
    __int8 state;

    /* ========= state =============
    0: not login
    [_#00] input name to login
    1: select ch
    [_#01] select ch to join
    2: in ch
    [_#02] select room to join
    [_#03] chat with in same channel
    [_#04] exit channel
    3: in room
    [_#05] chat with in same room
    [_#06] exit room
    ============================== */

    __int16 ch;
    __int16 room;
};

const char OPTION_VALUE = 1;
const int MAX_CLIENTS = 200;

const int MAX_CHANNEL = 20;
const int MAX_ROOM = 4;

// ch 1~5, room 1~5
std::vector<__int16> ch_vector[6];
std::vector<__int16> room_vector[6][6];

//Function Prototypes
int process_client(client_type &new_client, std::vector<client_type> &client_array, std::thread &thread);

int process_client(client_type &new_client, std::vector<client_type> &client_array, std::thread &thread)
{
    std::string msg = "";
    std::string msg_error = "";
    char tempmsg[DEFAULT_BUFLEN] = "";

    //Session
    while (1)
    {
        memset(tempmsg, 0, DEFAULT_BUFLEN);

        if (new_client.socket != 0)
        {
            int iResult = recv(new_client.socket, tempmsg, DEFAULT_BUFLEN, 0);

            if (iResult != SOCKET_ERROR)
            {
                if (strcmp("", tempmsg))
                    msg = "Client #" + std::to_string(new_client.id) + ": " + tempmsg;

                std::string recvmsg = tempmsg;
                std::string act = recvmsg.substr(0,4);
                std::string content = recvmsg.substr(4);

                std::cout << msg.c_str() << std::endl;

                if (act == "_#00")
                {
                    new_client.name = content;

                    new_client.state = 1;
                    iResult = send(client_array[new_client.id].socket, recvmsg.c_str(), strlen(recvmsg.c_str()), 0);
                }
                else if (act == "_#01") {
                    __int16 ch_num = atoi(content.c_str());
                    if (ch_vector[ch_num].size() < MAX_CHANNEL)
                    {
                        new_client.ch = ch_num;
                        ch_vector[ch_num].push_back(new_client.id);

                        new_client.state = 2;
                        iResult = send(client_array[new_client.id].socket, recvmsg.c_str(), strlen(recvmsg.c_str()), 0);
                    }
                    else {
                        msg_error = "Channel is full";
                        iResult = send(client_array[new_client.id].socket, msg_error.c_str(), strlen(msg_error.c_str()), 0);
                    }
                }
                else if (act == "_#02") {
                    __int16 room_num = atoi(content.c_str());
                    if (room_vector[new_client.ch][room_num].size() < MAX_ROOM)
                    {
                        new_client.room = room_num;
                        room_vector[new_client.ch][room_num].push_back(new_client.id);

                        new_client.state = 3;
                        iResult = send(client_array[new_client.id].socket, recvmsg.c_str(), strlen(recvmsg.c_str()), 0);
                    }
                    else {
                        msg_error = "Room is full";
                        iResult = send(client_array[new_client.id].socket, msg_error.c_str(), strlen(msg_error.c_str()), 0);
                    }
                }
                else if (act == "_#03") {

                }
                else if (act == "_#04") {
                    /* OUT CHANNEL */
                    auto it = std::find(ch_vector[new_client.ch].begin(),
                        ch_vector[new_client.ch].end(), new_client.id);
                    if (it != ch_vector[new_client.ch].end())
                        ch_vector[new_client.ch].erase(it);

                    for (auto i = ch_vector[new_client.ch].begin(); i != ch_vector[new_client.ch].end(); ++i)
                        std::cout << "Vector " << *i << ' ' << std::endl;

                    iResult = send(client_array[new_client.id].socket, recvmsg.c_str(), strlen(recvmsg.c_str()), 0);
                }
                else {
                    //Broadcast that message to the other clients
                    for (int i = 0; i < MAX_CLIENTS; i++)
                    {
                        if (client_array[i].socket != INVALID_SOCKET)
                            if (new_client.id != i)
                                iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
                    }
                }
               
            }
            else
            {
                msg = "Client #" + std::to_string(new_client.id) + " Disconnected";

                // ROOM DELETE FOR EXIT CLIENT //
                if (new_client.state == 3) {
                    auto it = std::find(room_vector[new_client.ch][new_client.room].begin(),
                        room_vector[new_client.ch][new_client.room].end(), new_client.id);
                    if (it != room_vector[new_client.ch][new_client.room].end())
                        room_vector[new_client.ch][new_client.room].erase(it);

                    for (auto i = room_vector[new_client.ch][new_client.room].begin(); i != room_vector[new_client.ch][new_client.room].end(); ++i)
                        std::cout << "Vector " << *i << ' ' << std::endl;
                }

                // CHANNEL DELETE FOR EXIT CLIENT //
                if (new_client.state >= 2) {
                    auto it = std::find(ch_vector[new_client.ch].begin(),
                        ch_vector[new_client.ch].end(), new_client.id);
                    if (it != ch_vector[new_client.ch].end())
                        ch_vector[new_client.ch].erase(it);

                    for (auto i = ch_vector[new_client.ch].begin(); i != ch_vector[new_client.ch].end(); ++i)
                        std::cout << "Vector " << *i << ' ' << std::endl;
                }

                // CLOSE SOCKET FOR EXIT CLIENT //
                std::cout << msg << std::endl;

                closesocket(new_client.socket);
                closesocket(client_array[new_client.id].socket);
                client_array[new_client.id].socket = INVALID_SOCKET;

                // Broadcast the disconnection message to the other clients
                
                for (int i = 0; i < MAX_CLIENTS; i++)
                {
                    if (client_array[i].socket != INVALID_SOCKET)
                        iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
                }

                break;
            }
        }
    } //end while

    thread.detach();

    return 0;
}

class Server
{
private:
    WSADATA wsaData;
    struct addrinfo hints;
    struct addrinfo *server = NULL;
    SOCKET server_socket = INVALID_SOCKET;
    std::string msg = "";
    std::vector<client_type> client;


    int num_clients = 0;
    int temp_id = -1;

    std::thread my_thread[MAX_CLIENTS];
    
public:
    Server() {
        client.resize(MAX_CLIENTS);
    };

    void initWinSock() {
        //Initialize Winsock
        std::cout << "Intializing Winsock..." << std::endl;
        WSAStartup(MAKEWORD(2, 2), &wsaData);

        //Setup hints
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        //Setup Server
        std::cout << "Setting up server..." << std::endl;
        getaddrinfo(IP_ADDRESS, DEFAULT_PORT, &hints, &server);

        //Create a listening socket for connecting to server
        std::cout << "Creating server socket..." << std::endl;
        server_socket = socket(server->ai_family, server->ai_socktype, server->ai_protocol);

        //Setup socket options
        setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &OPTION_VALUE, sizeof(int)); //Make it possible to re-bind to a port that was used within the last 2 minutes
        setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &OPTION_VALUE, sizeof(int)); //Used for interactive programs

                                                                                         //Assign an address to the server socket.
        std::cout << "Binding socket..." << std::endl;
        bind(server_socket, server->ai_addr, (int)server->ai_addrlen);

        //Listen for incoming connections.
        std::cout << "Listening..." << std::endl;
        listen(server_socket, SOMAXCONN);

        //Initialize the client list
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            client[i] = { INVALID_SOCKET, -1, "", "", 0, 0, 0 };
        }
    };

    void runServer() {
        while (1)
        {

            SOCKET incoming = INVALID_SOCKET;
            incoming = accept(server_socket, NULL, NULL);

            if (incoming == INVALID_SOCKET) continue;

            //Reset the number of clients
            num_clients = -1;

            //Create a temporary id for the next client
            temp_id = -1;
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if (client[i].socket == INVALID_SOCKET && temp_id == -1)
                {
                    client[i].socket = incoming;
                    client[i].id = i;
                    temp_id = i;
                }

                if (client[i].socket != INVALID_SOCKET)
                    num_clients++;

                //std::cout << client[i].socket << std::endl;
            }

            if (temp_id != -1)
            {
                //Send the id to that client
                std::cout << "Client #" << client[temp_id].id << " Accepted" << std::endl;
                msg = std::to_string(client[temp_id].id);
                send(client[temp_id].socket, msg.c_str(), strlen(msg.c_str()), 0);

                //Create a thread process for that client
                my_thread[temp_id] = std::thread(process_client, std::ref(client[temp_id]), std::ref(client), std::ref(my_thread[temp_id]));
            }
            else
            {
                msg = "Server is full";
                send(incoming, msg.c_str(), strlen(msg.c_str()), 0);
                std::cout << msg << std::endl;
            }
        } //end while
    }



    ~Server() {
        //Close listening socket
        closesocket(server_socket);

        //Close client socket
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            my_thread[i].detach();
            closesocket(client[i].socket);
        }

        //Clean up Winsock
        WSACleanup();
        std::cout << "Program has ended successfully" << std::endl;

        system("pause");
    }
};