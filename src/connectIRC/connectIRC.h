#pragma once

struct WSAData;
typedef unsigned long long pirc_socket;

class ConnectIRC {
private:
    WSAData* wsaData;
    unsigned short DllVersion;
    ConnectIRC();
public:
    static pirc_socket CreateSocket(const bool isNonBlocking = false);
    static void Connect(pirc_socket* connectingSocket, const bool secure, const char* address, const bool isServer);
    static char** ReceiveMsg(pirc_socket* receivingSocket, char* delimiter = nullptr);
    static void SendMsg(pirc_socket* sendingSocket, const char* message);
    static void Shutdown();
};
