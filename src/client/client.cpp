#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <iostream>>
#include "imgui.h"
#include "rlImGui.h"
#include "raylib.h"
#include "raymath.h"
#include "client.h"
#include "connectIRC.h"
#define PORT "6667"
#define URL "testnet.ergo.chat"

static  std::string connexionMessage = "";

Client::Client(float w,float h) : m_Width(w), m_Height(h)  {
    
}

void Client::Start(bool secure, const char* url) {
    bool m_Secure = secure;

    setConnexionState(AWAITING_CONNEXION);

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

	rlImGuiSetup(true);

   
  
}

void Client::Update() {

    if (m_connexionState == SENDING_CONNEXION_INFO_TO_SERVER)
    {
        WSAStartup(DllVersion, &wsaData);
        socket = ConnectIRC::CreateSocket();
        ConnectIRC::Connect(&socket, m_Secure, URL, false);
        u_long iMode = 1;
        ioctlsocket(socket, FIONBIO, &iMode);
        char nick[256] = {0};
        snprintf(nick, 256, "NICK %s\r\n", strNick);
        char *sendNick = nick;

        char user[256] = {0};
        snprintf(user, 256, "USER %s 0 **: %s\r\n", strNick, strUser);
        char *sendUser = user;

        ConnectIRC::SendMsg(&socket, sendNick);
        ConnectIRC::SendMsg(&socket, sendUser);

        setConnexionState(AWAIT_SERVER_ANSWER_TO_CONNEXION);
    }



    char **parsedResponse;
    parsedResponse = ConnectIRC::ReceiveMsg(&socket, "\r\n ");

    if (parsedResponse != nullptr) {
        if(m_connexionState == AWAIT_SERVER_ANSWER_TO_CONNEXION && strcmp(parsedResponse[1], "001") == 0){
            setConnexionState(CONNECTED_TO_SERVER);
        }
        else if (m_connexionState == AWAIT_SERVER_ANSWER_TO_CONNEXION && strcmp(parsedResponse[1], "001") != 0) {
            setConnexionState(AWAITING_CONNEXION);
        }

        char **ptr = parsedResponse;
        while (*ptr != nullptr) {
            
            std::cout << *ptr << std::endl;
            ++ptr;
        }
    }
}

void Client::Draw() {
    // Library/rllmGui/example/simple
    // https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html

    rlImGuiBegin();

    ImGui::SetWindowSize(ImVec2(m_Width, m_Height));
    ImGui::SetWindowPos(ImVec2(0,0));

    if (m_connexionState == AWAITING_CONNEXION || m_connexionState == SENDING_CONNEXION_INFO_TO_SERVER || m_connexionState == AWAIT_SERVER_ANSWER_TO_CONNEXION)
    {
        ImGui::SetCursorPos(ImVec2(50, 100));
        strNick[256];
        ImGui::InputTextWithHint("Nickname", "Input nickname here", strNick, IM_ARRAYSIZE(strNick));

        ImGui::SetCursorPos(ImVec2(50, 135));
        strUser[256];
        ImGui::InputTextWithHint("Username", "Input username here", strUser, IM_ARRAYSIZE(strUser));
        
        ImGui::SetCursorPos(ImVec2(50, 170));
        
        static int clicked = 0;
        static bool showConnexionMessage = false;

        if (ImGui::Button("Connexion", ImVec2(100, 40))){
            clicked++;
        }
        
        if(showConnexionMessage){
            
            ImGui::Text("%s" ,connexionMessage.c_str());
        }
        
        

        if (clicked)
        {
            bool invalidUsername = false;
            bool invalidNickname = false;

            if (strUser[0] == '\0' || isspace(strUser[0])) {
                invalidUsername = true;
            }

            if (strNick[0] == '\0' || isspace(strUser[0])) {
                invalidNickname = true;
            }

            if (invalidUsername && invalidNickname)
            {
                connexionMessage = "Invalid Username and Nickname.";
                showConnexionMessage = true;
            }
            else if (invalidNickname)
            {
                connexionMessage = ("Invalid Nickname.");
                showConnexionMessage = true;
            }
            else if (invalidUsername)
            {
                connexionMessage = ("Invalid Username.");
                showConnexionMessage = true;
            }
            else{
                setConnexionState(SENDING_CONNEXION_INFO_TO_SERVER);
                connexionMessage = ("Awaiting for server answer.");
                showConnexionMessage = true;
            }

            clicked = 0;
        }
	}    
    else if(m_connexionState == CONNECTED_TO_SERVER){
        ImGui::SetCursorPos(ImVec2(50, 100));
        static char strNick[256];
        ImGui::InputTextWithHint("Nicknane", "Input nickname here", strNick, IM_ARRAYSIZE(strNick));

        ImGui::SetCursorPos(ImVec2(50, 135));
        static char strUser[256];
        ImGui::InputTextWithHint("Username", "Input username here", strUser, IM_ARRAYSIZE(strUser));
    }
    rlImGuiEnd();
}

void Client::End() {
    rlImGuiShutdown();
    ConnectIRC::Shutdown();
}

void Client::setConnexionState(ClientState state){
    m_connexionState = state;
}