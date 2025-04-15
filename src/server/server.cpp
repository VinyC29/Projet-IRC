#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
 
#include "server.h"
#include "connectIRC.h"

#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>

void PrintParsedMessages(char** parsedResponse) {
    for (int i = 0; parsedResponse[i] != nullptr; i++) {
        printf("%d | %s\n", i, parsedResponse[i]);
    }
}

char* ProcessMessage(char** parsedResponse) {
    PrintParsedMessages(parsedResponse);

    char response[1024] = "ERROR";

    if (strcmp(parsedResponse[3], "NICK") == 0) {

        char* nick = parsedResponse[4];

        strcpy(response, ":projectirc.example.com 001 ");    
        strcat(response, nick);  
        strcat(response, " :Welcome to the IRC Project\r\n");
      
    }

    return response;
}

/* -------- ↓ ----------- ↓ -------- */
/* -------- ↓ APP METHODS ↓ -------- */
/* -------- ↓ ----------- ↓ -------- */

void Server::Start(bool secureBoolean, const char* url) {
    
    serverSocket = ConnectIRC::CreateSocket();
    ConnectIRC::Connect(&serverSocket, secureBoolean, url, true);

    listen(serverSocket, SOMAXCONN); 
    printf("Waiting for clients...\n");

    SOCKADDR_IN clientAddr;
    int clientAddrLength = sizeof(clientAddr);

    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLength);

    u_long mode = 1;
    ioctlsocket(clientSocket, FIONBIO, &mode); // Stop the client socket from blocking the execution

    printf("Client connected\n");

}

void Server::Update() {

    char** parsedResponse;
    parsedResponse = ConnectIRC::ReceiveMsg(&clientSocket, "\r\n ");

    if (parsedResponse != nullptr) {

        char* response = ProcessMessage(parsedResponse);

        ConnectIRC::SendMsg(&clientSocket, response);
        printf("Server sent message | %s", response);

    } else {
        printf("No message received\n");
    }
    
}

void Server::Draw() {

}

void Server::End() {
    ConnectIRC::Shutdown();
}
