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

ConnectIRC::ConnectIRC() noexcept {
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

void ConnectIRC::Connect(const bool secure, const char* address, const bool isServer) {

    int port = 6667;
    if (secure) { port = 6697; }
    
    if (isServer) {

        SOCKET serverSocket = CreateSocket();
        SOCKADDR_IN sin;

        ZeroMemory(&sin, sizeof(sin));
        sin.sin_port = htons(port);
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr(address);

        bind(serverSocket, (const sockaddr*)&sin, sizeof(sin));
    
    } else {

        SOCKET ClientSocket = CreateSocket();
        struct addrinfo *ptr = NULL;
        struct addrinfo hints;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        
        getaddrinfo(address, (PCSTR)port, &hints, &ptr);
        
        connect(ClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

        freeaddrinfo(ptr);

    }
}

void ConnectIRC::ReceiveMessage(Knob_String_Builder* StringBuilder) {
    Knob_String_Builder testSB;

    testSB.capacity = 16;
    testSB.count = 0;
    testSB.items = (char*)malloc(testSB.capacity * sizeof(char));
    testSB.items[0] = '\0';

    const char *buffer1 = "Hello, ";
    const char *buffer2 = "world!";

    // I wasn't able to fix the problem here, will be fixed later :(
    knob_sb_append_buf(&testSB, buffer1, strlen(buffer1)); // ! a value of type "void *" cannot be assigned to an entity of type "char *" !
    knob_sb_append_buf(&testSB, buffer2, strlen(buffer2)); // ! a value of type "void *" cannot be assigned to an entity of type "char *" !

    printf(testSB.items);
}

void ConnectIRC::SendMessage(Knob_String_Builder* StringBuilder) {

}

void ConnectIRC::Shutdown() {
    WSACleanup();
}