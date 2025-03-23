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
    static SOCKET CreateSocket();
    ConnectIRC() noexcept;
public:
    static void Connect(const bool secure, const char* address, const bool isServer);
    static void ReceiveMessage(Knob_String_Builder* StringBuilder);
    static void SendMessage(Knob_String_Builder* StringBuilder);
    static void Shutdown();
};
