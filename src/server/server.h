#pragma once

#include "IRC_Interface.h";
#include <WinSock2.h>

class Server : public IRC_Interface {
private:

    Server() {
        WSAData wsaData;
        WORD DllVersion = MAKEWORD(2, 1);
        WSAStartup(DllVersion, &wsaData);
    }

    SOCKET serverSocket;
    SOCKET clientSocket;

public:
    void Start(bool secure, const char* url) override;
    void Update() override;
    void Draw() override;
    void End() override;
};
