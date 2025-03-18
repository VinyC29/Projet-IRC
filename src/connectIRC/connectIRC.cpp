#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "connectIRC.h"
#include "knob.h"

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

ConnectIRC::ConnectIRC() {
    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 1);

    WSAStartup(DllVersion, &wsaData);
    
    
}

SOCKET ConnectIRC::CreateSocket() {
    
    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 1);

    WSAStartup(DllVersion, &wsaData);
    
    // Create Socket
    int iFamily = AF_INET;
    int iType = SOCK_STREAM;
    int iProtocol = IPPROTO_TCP;

    SOCKET sock = socket(iFamily, iType, iProtocol);
    return sock;
}

void ConnectIRC::Connect(const bool secure, const char* address) {
    
}