#pragma once

#include <WinSock2.h>
#include "IRC_Interface.h"
#include "vector"
#include "string"

using namespace std;

typedef struct Channel {
    string ChannelName;
    string FilePath;
    FILE* MessageHistory;
    std::vector<string> ClientNames;
};

class Server : public IRC_Interface {
private:

    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 1);

    SOCKET serverSocket;
    SOCKET clientSocket;
    char* clientNickname;

    vector<Channel> channels;

    char* ProcessMessage(char** parsedResponse);

    float m_Width;
    float m_Height;

public:
    Server(float width,float height);

    void Start(bool secure, const char* url) override;
    void Update() override;
    void Draw() override;
    void End() override;
};

char* ProcessMessage(char** parsedResponse);