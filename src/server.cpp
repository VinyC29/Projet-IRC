#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define PORT 6697

int main() {
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
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

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

        printf("Client Response: %s", szResponse);

		// Test server answer
        char* answerMessage = "TEST WOOHOO!";
        send(clientSock, answerMessage, strlen(answerMessage), 0);
    }

    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();

    ExitProcess(EXIT_SUCCESS);
}
