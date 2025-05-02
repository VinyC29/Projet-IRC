#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <iostream>
#include "imgui.h"
#include "rlImGui.h"
#include "raylib.h"
#include "raymath.h"
#include "client.h"
#include "connectIRC.h"
#define PORT "6667"
#define URL "testnet.ergo.chat"

static  std::string connexionMessage = "";

// testnet.ergo doit attendre 15 sec pour avoir la liste des channels
bool haveChannel = false;
bool listChannel = false;

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

    if(m_connexionState == CONNECTED_TO_SERVER && !haveChannel){
        Sleep(15000);
        char seeChannel[256] = {0};
        snprintf(seeChannel, 256, "LIST\r\n");
        ConnectIRC::SendMsg(&socket, seeChannel);
        haveChannel = true;
        listChannel = true;
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

        if(strcmp(parsedResponse[0], "PING") == 0){
            char pingAnswer[256] = {0};
            snprintf(pingAnswer, 256, "PONG :%s\r\n", parsedResponse[1]);
            ConnectIRC::SendMsg(&socket, pingAnswer);
        }

        if(strcmp(parsedResponse[1], "322") == 0){
            int channelCount = 0;
            char **prtchannel = parsedResponse;

            while (*prtchannel != nullptr) {
                if (strcmp(*prtchannel, "322") == 0) {
                    char* channelToken = *(prtchannel + 2);
                    channels[channelCount] = strdup(channelToken + 1);
                    channelCount++;
                }
                ++prtchannel;  
            }
            haveChannel = true;             
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
    
            // BeginListBox() is essentially a thin wrapper to using BeginChild()/EndChild()
            // using the ImGuiChildFlags_FrameStyle flag for stylistic changes + displaying a label.
            // You may be tempted to simply use BeginChild() directly. However note that BeginChild() requires EndChild()
            // to always be called (inconsistent with BeginListBox()/EndListBox()).

            // Using the generic BeginListBox() API, you have full control over how to display the combo contents.
            // (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
            // stored in the object itself, etc.)
            static char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
            static int item_selected_idx = 0; // Here we store our selected data as an index.

            static bool item_highlight = true;
            int item_highlighted_idx = -1; // Here we store our highlighted data as an index.

            if(haveChannel){
                ImGui::Text("Available channels");
                if (ImGui::BeginListBox("##listbox 2", ImVec2(200, 5 * ImGui::GetTextLineHeightWithSpacing())))
                {
                    for (int n = 0; n < IM_ARRAYSIZE(channels); n++)
                    {
                        bool is_selected = (item_selected_idx == n);
                        ImGuiSelectableFlags flags = (item_highlighted_idx == n) ? ImGuiSelectableFlags_Highlight : 0;
                        if (ImGui::Selectable(channels[n], is_selected, flags))
                            item_selected_idx = n;

                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndListBox();
            }
        }
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