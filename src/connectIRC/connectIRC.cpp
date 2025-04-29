#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "connectIRC.h"

#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

ConnectIRC::ConnectIRC() {
    static WSAData data = {0};
    wsaData = &data;
    DllVersion = MAKEWORD(2, 1);
    WSAStartup(DllVersion, wsaData);  
}

pirc_socket ConnectIRC::CreateSocket(const bool isNonBlocking) {
    
    // Create Socket
    int iFamily = AF_INET;
    int iType = SOCK_STREAM;
    int iProtocol = IPPROTO_TCP;

    pirc_socket sock = socket(iFamily, iType, iProtocol);
    if (isNonBlocking)
    {
        u_long iMode = 1; // Mode pour le socket 0 pour blocant 1 pour non blocant
        int iResult = ioctlsocket(sock, FIONBIO, &iMode);
    }
    return sock;
}


void ConnectIRC::Connect(pirc_socket* connectingSocket, const bool secure, const char* address, const bool isServer) {

    int port = 6667;
    if (secure) { port = 6697; }
    
    if (isServer) {

        SOCKADDR_IN sin;

        ZeroMemory(&sin, sizeof(sin));
        sin.sin_port = htons(port);
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr(address);

        bind(*connectingSocket, (const sockaddr*)&sin, sizeof(sin));
    
    } else {

        struct addrinfo *ptr = NULL;
        struct addrinfo hints;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        
        char portStr[6];
        snprintf(portStr, sizeof(portStr), "%d", port);

        getaddrinfo(address, portStr, &hints, &ptr);

        connect(*connectingSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

        freeaddrinfo(ptr);

    }
}

char** ConnectIRC::ReceiveMsg(pirc_socket* receivingSocket, char* delimiter) {

    char szBuffer[2048];
    char szResponse[2048];
    char* parsedResponse[1024];

    for (int i = 0; i < sizeof(szBuffer); i++) {
        szBuffer[i] = '\0';
    }

    int bytesReceived = recv(*receivingSocket, szResponse, sizeof(szResponse) - 1, 0);

    if (bytesReceived <= 0) { return nullptr; } // Connection was closed or did not receive anything

    szResponse[bytesReceived] = '\0'; // Wipe response
    strcat(szBuffer, szResponse); // Append to buffer

    char* token = strtok(szResponse, delimiter); // Parse response
    int i = 0;

    while (token != NULL)
    {
        parsedResponse[i] = token; // Put the parsed strings in the parsedResponse array with an index.
        token = strtok(NULL, delimiter);
        i++;
    }

    parsedResponse[i] = nullptr; // End the array with nullptr for a condition
    return parsedResponse;

}
    
void ConnectIRC::SendMsg(pirc_socket* sendingSocket, const char* message) {
    send(*sendingSocket, message, strlen(message), 0);
}

void ConnectIRC::Shutdown() {
    WSACleanup();
}