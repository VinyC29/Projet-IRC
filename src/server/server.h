#pragma once

#include <WinSock2.h>
#include "IRC_Interface.h"
#include "vector"

using namespace std;

typedef struct Channel {
    char* ChannelName;
    char* FilePath;
    FILE* MessageHistory;
    vector<char*> ClientNames;
};

class Server : public IRC_Interface {
private:

    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 1);

    SOCKET serverSocket;
    SOCKET clientSocket;

    vector<Channel> channels;

public:

    Server() {
        WSAStartup(DllVersion, &wsaData);
    }

    void Start(bool secure, const char* url) override;
    void Update() override;
    void Draw() override;
    void End() override;
};
