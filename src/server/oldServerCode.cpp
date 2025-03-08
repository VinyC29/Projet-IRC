/* --------------------------------------------- */
/* THIS FILE IS TO BE REFACTORED INTO server.cpp */
/* --------------------------------------------- */

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "server.h"

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define PORT 6667
#define ADDRESS "127.0.0.1"

int ServerStart() {
    // Init WINSOCK
    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 1);

    WSAStartup(DllVersion, &wsaData);

    // Create Socket
    int iFamily = AF_INET;
    int iType = SOCK_STREAM;
    int iProtocol = IPPROTO_TCP;

    SOCKET serverSock = socket(iFamily, iType, iProtocol);

    // Define server info
    SOCKADDR_IN sin;
    ZeroMemory(&sin, sizeof(sin));
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ADDRESS);

    // Bind the socket
    bind(serverSock, (const sockaddr*)&sin, sizeof(sin));

    // Wait for client sockets
    listen(serverSock, SOMAXCONN); 
    printf("Waiting for clients...\n");

    // Accept client
    SOCKET clientSock;
    SOCKADDR_IN clientAddr;
    int clientAddrLength = sizeof(clientAddr);

    clientSock = accept(serverSock, (sockaddr*)&clientAddr, &clientAddrLength);

    printf("Client connected\n");

    char szBuffer[4096];
    char szResponse[4096];
    char* parsedResponse[512];

    // Wipe buffer
    for (int i = 0; i < sizeof(szBuffer); i++) {
        szBuffer[i] = '\0';
    }

    // Receive data from the client
    while (true) {
        
		int bytesReceived = recv(clientSock, szResponse, sizeof(szResponse) - 1, 0);
		if (bytesReceived <= 0) {
            break; // Connection closed
        }

		// Client response
        szResponse[bytesReceived] = '\0'; // Wipe response
        strcat(szBuffer, szResponse); // Append to buffer

        //printf(szResponse); // Client response

        char* delimiter = "\r\n ";
        char* token = strtok(szResponse, delimiter); // Parse client response
        int i = 0;

        while (token != NULL)
        {
            parsedResponse[i] = token;
            printf("%d : %s\n", i, parsedResponse[i]);  // Put the parsed strings in the parsedResponse list with an index.
            token = strtok(NULL, delimiter);
            i++;
        }
        
        printf("\n--- Server doing its thing... ---\n");

		// Server answer
        char answerMessage[1024];

        if (strcmp(parsedResponse[0], "NICK") == 0) {
            char* nickname = parsedResponse[1];
            char* username = parsedResponse[3];
            strcpy(answerMessage, "Hello world!");
        }

        printf(answerMessage);
        send(clientSock, answerMessage, strlen(answerMessage), 0);
    }

    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();
}