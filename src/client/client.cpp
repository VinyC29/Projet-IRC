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
    CONNEXION,
    CONNECTED_TO_SERVER
};

ClientState connexionstate = CONNEXION;

Client::Client(float w,float h) : m_Width(w), m_Height(h)  {

}
void Client::Start(bool secure, const char* url) {

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

	rlImGuiSetup(true);

}

void Client::Update() {

}

void Client::Draw() {
    // Library/rllmGui/example/simple


    rlImGuiBegin();
    ImGui::SetWindowSize(ImVec2(m_Width, m_Height));
    ImGui::SetWindowPos(ImVec2(0,0));
    if (connexionstate == CONNEXION)
    {
        ImGui::SetCursorPos(ImVec2(50, 100));
        static char strNick[256];
        ImGui::InputTextWithHint("Nicknane", "Input nickname here", strNick, IM_ARRAYSIZE(strNick));

        ImGui::SetCursorPos(ImVec2(50, 135));
        static char strUser[256];
        ImGui::InputTextWithHint("Username", "Input username here", strUser, IM_ARRAYSIZE(strUser));
        
        ImGui::SetCursorPos(ImVec2(50, 170));
        static int clicked = 0;
        static bool showErrorMessage = false;
        static  std::string errorMessage = "";
        
        if (ImGui::Button("isConnected", ImVec2(100, 40))){
            clicked++;
        }
        
        if(showErrorMessage){
            
            ImGui::Text("%s" ,errorMessage.c_str());
        }

        if (clicked)
        {
            bool invalidUsername = IsStringNullOrEmpty(strUser);
            bool invalidNickname = IsStringNullOrEmpty(strNick);


            if (invalidUsername && invalidNickname)
            {
                errorMessage = "Invalid Username and Nickname.";
                showErrorMessage = true;
            }
            else if (invalidNickname)
            {
                errorMessage = ("Invalid Nickname.");
                showErrorMessage = true;
            }
            else if (invalidUsername)
            {
                errorMessage = ("Invalid Username.");
                showErrorMessage = true;
            }
            else{
                connexionstate = CONNECTED_TO_SERVER;
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