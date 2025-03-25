#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "connectIRC.h"
#include "knob.h"

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

ConnectIRC::ConnectIRC() {
    DllVersion = MAKEWORD(2, 1);
    WSAStartup(DllVersion, &wsaData);  
}

SOCKET ConnectIRC::CreateSocket() {
    
    // Create Socket
    int iFamily = AF_INET;
    int iType = SOCK_STREAM;
    int iProtocol = IPPROTO_TCP;

    SOCKET sock = socket(iFamily, iType, iProtocol);
    return sock;
}

void ConnectIRC::Connect(SOCKET* connectingSocket, const bool secure, const char* address, const bool isServer) {

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
        
        getaddrinfo(address, (PCSTR)port, &hints, &ptr);
        
        connect(*connectingSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

        freeaddrinfo(ptr);

    }
}

char** ConnectIRC::ReceiveMessage(SOCKET* receivingSocket, char* delimiter) {

    char szBuffer[2048];
    char szResponse[2048];
    char* parsedResponse[1024];

    for (int i = 0; i < sizeof(szBuffer); i++) {
        szBuffer[i] = '\0';
    }

    int bytesReceived = recv(*receivingSocket, szResponse, sizeof(szResponse) - 1, 0);

    if (bytesReceived <= 0) { return; } // Connection was closed

    szResponse[bytesReceived] = '\0'; // Wipe response
    strcat(szBuffer, szResponse); // Append to buffer

    char* token = strtok(szResponse, delimiter); // Parse response
    int i = 0;

    while (token != NULL)
    {
        parsedResponse[i] = token;
        printf("%d : %s\n", i, parsedResponse[i]);  // Put the parsed strings in the parsedResponse array with an index.
        token = strtok(NULL, delimiter);
        i++;
    }

    return parsedResponse;

}
    
void ConnectIRC::SendMessage(Knob_String_Builder* StringBuilder) {
    
}

void ConnectIRC::Shutdown() {
    WSACleanup();
}