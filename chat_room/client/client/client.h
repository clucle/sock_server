#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512            
#define IP_ADDRESS "127.0.0.1"
#define DEFAULT_PORT "3504"

struct client_type
{
    SOCKET socket;
    int id;
    char received_message[DEFAULT_BUFLEN];
    string name;
    int state;
};
client_type client = { INVALID_SOCKET, -1, "", "", 0 };

int process_client();

int process_client()
{
    while (1)
    {
        memset(client.received_message, 0, DEFAULT_BUFLEN);
        if (client.socket != 0)
        {
            int iResult = recv(client.socket, client.received_message, DEFAULT_BUFLEN, 0);

            if (iResult != SOCKET_ERROR) {
                string recvmsg = client.received_message;
                string act = recvmsg.substr(0,4);
                string content = recvmsg.substr(4);

                if (act == "_#00") {
                    client.name = content;
                    client.state = 1;
                    cout << client.name << " Log In to World!" << endl;
                }

            }
            else
            {
                cout << "recv() failed: " << WSAGetLastError() << endl;
                break;
            }
        }
    }

    if (WSAGetLastError() == WSAECONNRESET)
        cout << "The server has disconnected" << endl;

    return 0;
}

class Client
{
private:
    WSAData wsa_data;
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    string sent_message = "";
    
    int iResult = 0;
    string message;

    boolean WinSockStartup() {
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (iResult != 0) {
            cout << "WSAStartup() failed with error: " << iResult << endl;
            return false;
        }
        return true;
    }
    boolean getAddrInfo() {
        // Resolve the server address and port
        iResult = getaddrinfo(IP_ADDRESS, DEFAULT_PORT, &hints, &result);
        if (iResult != 0) {
            cout << "getaddrinfo() failed with error: " << iResult << endl;
            WSACleanup();
            system("pause");
            return false;
        }
        return true;
    }
    boolean connectSocket() {

        // Attempt to connect to an address until one succeeds
        for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

            // Create a SOCKET for connecting to server
            client.socket = socket(ptr->ai_family, ptr->ai_socktype,
                ptr->ai_protocol);
            if (client.socket == INVALID_SOCKET) {
                cout << "socket() failed with error: " << WSAGetLastError() << endl;
                WSACleanup();
                system("pause");
                return 1;
            }

            // Connect to server.
            iResult = connect(client.socket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                closesocket(client.socket);
                client.socket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        freeaddrinfo(result);

        if (client.socket == INVALID_SOCKET) {
            cout << "Unable to connect to server!" << endl;
            WSACleanup();
            system("pause");
            return 1;
        }

        cout << "Successfully Connected" << endl;
    }

    string printUserAct(int state) {
        string input;
        string result_string;

        switch (state) {
        case 0:
            cout << "Input Your Name : ";
            result_string = "_#00";
            getline(cin, input);
            result_string += input;
            break;
        case 1:
            cout << "Select Channel [1~20] : ";
            result_string = "_#01";
            getline(cin, input);
            result_string += input;
            break;
        default:
            cout << "ERROR CASE" << endl;
            break;
        }
        return result_string;
    }

public:
    int initWinSock() {
        cout << "Starting Client...\n";

        if (!WinSockStartup()) return -1;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        cout << "Connecting...\n";

        if (!getAddrInfo()) return -1;
        if (!connectSocket()) return -1;
    }


    void runClient() {
        //Obtain id from server for this client;
        recv(client.socket, client.received_message, DEFAULT_BUFLEN, 0);
        message = client.received_message;

        if (message != "Server is full")
        {
            client.id = atoi(client.received_message);
            client.state = 0;

            thread my_thread(process_client);

            while (1)
            {
                sent_message = printUserAct(client.state);
                cout << client.socket << endl;
                iResult = send(client.socket, sent_message.c_str(), strlen(sent_message.c_str()), 0);
                if (iResult <= 0)
                {
                    cout << "send() failed: " << WSAGetLastError() << endl;
                    break;
                }
                Sleep(10);
            }

            //Shutdown the connection since no more data will be sent
            my_thread.detach();
        }
        else
            cout << client.received_message << endl;

        cout << "Shutting down socket..." << endl;
        iResult = shutdown(client.socket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            cout << "shutdown() failed with error: " << WSAGetLastError() << endl;
            closesocket(client.socket);
            WSACleanup();
            system("pause");
        }
    }

    ~Client() {
        closesocket(client.socket);
        WSACleanup();
        system("pause");
    }

};