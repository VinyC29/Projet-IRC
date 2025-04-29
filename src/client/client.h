#pragma once
#include "IRC_Interface.h"
#include <WinSock2.h>

class Client : public IRC_Interface {
private:
    float m_Width;
    float m_Height;
    char strNick[256] = {0};
    char strUser[256] = {0};
    SOCKET socket;
    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 1);

public:
    Client(float width,float height);
    void Start(bool secure, const char* url) override;
    void Update() override;
    void Draw() override;
    void End() override;
};
