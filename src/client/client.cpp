#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include "imgui.h"
#include "rlImGui.h"
#include "raylib.h"
#include "raymath.h"
#include "verificator.h"
#include "client.h"
#include <cstring>
#include <string>
#define PORT 6667

enum ClientState
{
    AWAITING_CONNEXION,
    AWAIT_SERVER_ANSWER_TO_CONNEXION,
    CONNECTED_TO_SERVER
};

ClientState connexionstate = AWAITING_CONNEXION;

Client::Client(float w,float h) : m_Width(w), m_Height(h)  {

}
void Client::Start(const char* url) {

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

	rlImGuiSetup(true);

}

void Client::Update() {
    
    if(connexionstate == AWAIT_SERVER_ANSWER_TO_CONNEXION){
        printf("hi");
    }
}

void Client::Draw() {
    // Library/rllmGui/example/simple
    // https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html

    rlImGuiBegin();
    ImGui::SetWindowSize(ImVec2(m_Width, m_Height));
    ImGui::SetWindowPos(ImVec2(0,0));
    if (connexionstate == AWAITING_CONNEXION || connexionstate == AWAIT_SERVER_ANSWER_TO_CONNEXION)
    {
        ImGui::SetCursorPos(ImVec2(50, 100));
        static char strNick[256];
        ImGui::InputTextWithHint("Nicknane", "Input nickname here", strNick, IM_ARRAYSIZE(strNick));

        ImGui::SetCursorPos(ImVec2(50, 135));
        static char strUser[256];
        ImGui::InputTextWithHint("Username", "Input username here", strUser, IM_ARRAYSIZE(strUser));
        
        ImGui::SetCursorPos(ImVec2(50, 170));
        static int clicked = 0;
        static bool showConnexionMessage = false;
        static  std::string connexionMessage = "";
        
        if (ImGui::Button("Connexion", ImVec2(100, 40))){
            clicked++;
        }
        
        if(showConnexionMessage){
            
            ImGui::Text("%s" ,connexionMessage.c_str());
        }
        

        if (clicked)
        {
            bool invalidUsername = IsStringNullOrEmpty(strUser);
            bool invalidNickname = IsStringNullOrEmpty(strNick);


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
                connexionstate = AWAIT_SERVER_ANSWER_TO_CONNEXION;
                connexionMessage = ("Awaiting for server answer.");
                showConnexionMessage = true;
            }

            clicked = 0;
        }
	}    
    else if(connexionstate == CONNECTED_TO_SERVER){
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
}