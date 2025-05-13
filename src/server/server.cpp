#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
 
#include "server.h"
#include "connectIRC.h"

#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>

// #include "rlImGui.h"
#include "imgui.h"

Server::Server(float w,float h) : m_Width(w), m_Height(h)  {
    WSAStartup(DllVersion, &wsaData);
}

void PrintParsedMessages(char** parsedResponse) {
    for (int i = 0; parsedResponse[i] != nullptr; i++) {
        printf("%d | %s\n", i, parsedResponse[i]);
    }
}

Channel CreateChannel(const char* newChannelName) {
    Channel newChannel;

    newChannel.ChannelName = string(newChannelName);

    newChannel.FilePath = newChannel.ChannelName + ".txt";

    newChannel.MessageHistory = fopen(newChannel.FilePath.c_str(), "w");
    if (newChannel.MessageHistory == nullptr) {
        perror("Error creating file");
    }

    newChannel.ClientNames.clear();

    return newChannel;
}


void WriteFileMessage(Channel channel, char* message) {
    if (channel.MessageHistory != nullptr) {
        fflush(channel.MessageHistory);
    }
}

char* Server::ProcessMessage(char** parsedResponse) {
    PrintParsedMessages(parsedResponse);

    char response[1024] = "ERROR";

    if (strcmp(parsedResponse[0], "NICK") == 0) {

        char* nick = parsedResponse[3];

        strcpy(response, ":projectirc.example.com 001 ");    
        strcat(response, nick);  
        strcat(response, " :Welcome to the IRC Project\r\n");
      
        clientNickname = nick;
    }

    if (strcmp(parsedResponse[0], "JOIN") == 0) {

        char* channel = parsedResponse[1];

        strcpy(response, clientNickname);
        strcat(response, "\r\nJOIN ");  
        strcat(response, channel);

        for (int i = 0; i < channels.size(); i++){
            if (channels[i].ChannelName == channel) {
                channels[i].ClientNames.push_back(clientNickname);
            }
        }
    }

    if (strcmp(parsedResponse[0], "PART") == 0) {

        char* channel = parsedResponse[1];

        strcpy(response, clientNickname);
        strcat(response, "\r\nPART ");  
        strcat(response, channel);

        for (int i = 0; i < channels.size(); i++){
            if (channels[i].ChannelName == channel) {
                channels[i].ClientNames.erase(std::remove(channels[i].ClientNames.begin(), channels[i].ClientNames.end(), clientNickname), channels[i].ClientNames.end());
            }
        }
    }

    return response;
}

/* -------- ↓ ----------- ↓ -------- */
/* -------- ↓ APP METHODS ↓ -------- */
/* -------- ↓ ----------- ↓ -------- */

void Server::Start(bool secureBoolean, const char* url) {
    Channel myChannel = CreateChannel("#Default");
    Channel myChannel1 = CreateChannel("#DefaultAlt");
    channels.push_back(myChannel);
    channels.push_back(myChannel1);

    //WriteFileMessage(channels.at(0), "Yooo");     // This won't work and I have no idea why
    //WriteFileMessage(channels.at(0), "Heyyy");    // This won't work and I have no idea why
    
    serverSocket = ConnectIRC::CreateSocket();
    ConnectIRC::Connect(&serverSocket, secureBoolean, url, true);

    listen(serverSocket, SOMAXCONN); 
    printf("Waiting for clients...\n");

    SOCKADDR_IN clientAddr;
    int clientAddrLength = sizeof(clientAddr);

    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLength);

    u_long mode = 1;
    ioctlsocket(clientSocket, FIONBIO, &mode);

    printf("Client connected\n");

}

void Server::Update() {

    char** parsedResponse;
    parsedResponse = ConnectIRC::ReceiveMsg(&clientSocket, "\r\n ");

    if (parsedResponse != nullptr) {

        char* response = ProcessMessage(parsedResponse);

        ConnectIRC::SendMsg(&clientSocket, response);
        printf("Server sent message | %s", response);

    } else {
        printf("No message received\n");
    }
    
}

void Server::Draw() {
    ImGui::SetWindowSize(ImVec2(m_Width, m_Height));
    ImGui::SetWindowPos(ImVec2(0, 0));

    ImGui::Text("%s", "Existing Channels:");

    for (int i = 0; i < channels.size(); i++) {

        ImGui::BulletText("%s", channels[i].ChannelName.c_str());

        if (!channels[i].ClientNames.empty()) {
            ImGui::Indent();
            ImGui::Text("Connected Clients:");

            for (int j = 0; j < channels[i].ClientNames.size(); j++) {
                ImGui::BulletText("%s", channels[i].ClientNames[j].c_str());
            }

            ImGui::Unindent();
        } else {
            ImGui::Text("No clients connected.");
        }
    }
}




void Server::End() {
    ConnectIRC::Shutdown();
}
