#pragma once
#include "IRC_Interface.h"
#include <WinSock2.h>

enum ClientState
{
    AWAITING_CONNEXION,
    SENDING_CONNEXION_INFO_TO_SERVER,
    AWAIT_SERVER_ANSWER_TO_CONNEXION,
    CONNECTED_TO_SERVER
};



class Client : public IRC_Interface {
private:
    ClientState m_connexionState = AWAITING_CONNEXION;
    float m_Width;
    float m_Height;
    bool m_Secure;
    char strNick[256] = {0};
    char strUser[256] = {0};
    SOCKET socket;
    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 1);
    void setConnexionState(ClientState state);

public:
    Client(float width,float height);
    void Start(bool secure, const char* url) override;
    void Update() override;
    void Draw() override;
    void End() override;
};
