#pragma once

#include <WinSock2.h>

class ConnectIRC {
private:
    WSAData wsaData;
    WORD DllVersion;
    ConnectIRC();
public:
    static SOCKET CreateSocket();
    static void Connect(SOCKET* connectingSocket, const bool secure, const char* address, const bool isServer);
    static char** ReceiveMsg(SOCKET* receivingSocket, char* delimiter = nullptr);
    static void SendMsg(SOCKET* sendingSocket, const char* message);
    static void Shutdown();
};
