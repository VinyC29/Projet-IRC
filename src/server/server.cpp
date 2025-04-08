#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
 
#include "server.h"
#include "connectIRC.h"

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void Server::Start(bool secureBoolean, const char* url) {

    serverSocket = ConnectIRC::CreateSocket();
    ConnectIRC::Connect(&serverSocket, secureBoolean, url, true);

    listen(serverSocket, SOMAXCONN); 
    printf("Waiting for clients...\n");

    SOCKADDR_IN clientAddr;
    int clientAddrLength = sizeof(clientAddr);

    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLength);

    printf("Client connected\n");

}

void Server::Update() {

}

void Server::Draw() {

}

void Server::End() {
    ConnectIRC::Shutdown();
}