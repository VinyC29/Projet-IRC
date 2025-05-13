#pragma once
#include "IRC_Interface.h"
#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <string>

enum ClientState
{
    AWAITING_CONNEXION,
    SENDING_CONNEXION_INFO_TO_SERVER,
    AWAIT_SERVER_ANSWER_TO_CONNEXION,
    CONNECTED_TO_SERVER,
    JOINNIG_CHANNEL
};



class Client : public IRC_Interface {
private:
    ClientState m_connexionState = AWAIT_SERVER_ANSWER_TO_CONNEXION;
    float m_Width;
    float m_Height;
    int channelCount = 0;
    int chatHistoryCount = 0;
    int channelUsers = 0;
    bool m_Secure;
    bool m_Channel = false;
    bool m_FirstJoin = true; 
    bool m_SendingMsg = false;
    char strChannel[256] = {0};
    char strOldChannel[256] = {0};
    char strNick[256] = {0};
    char strUser[256] = {0};
    char strMsg [256] = {0};
    std::vector<std::string> channels;
    char *chatHistory [256] = {0};
    char *userChannels[256] = {0};
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
