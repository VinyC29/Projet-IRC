#pragma once

#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>
#include <Windows.h>

class ConnectIRC {
private:
    WSAData wsaData;
    WORD DllVersion;
    static int iResult;  
    static SOCKET CreateSocket();
public:
    static void Connect(const bool secure, const char* address, const bool isServer);
};
