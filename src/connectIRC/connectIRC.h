#pragma once

#include "knob.h"

#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>
#include <Windows.h>

class ConnectIRC {
private:
    WSAData wsaData;
    WORD DllVersion;
    ConnectIRC();
public:
    static SOCKET CreateSocket();
    static void Connect(SOCKET* connectingSocket, const bool secure, const char* address, const bool isServer);
    static char** ReceiveMessage(SOCKET* receivingSocket, char* delimiter = nullptr);
    static void SendMessage(Knob_String_Builder* StringBuilder);
    static void Shutdown();
};
