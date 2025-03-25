#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include "imgui.h"
#include "rlImGui.h"
#include "raylib.h"
#include "raymath.h"
#include "client.h"
#include <cstring>
#include <string>

#define PORT 6667
    int Connexion = 0;

void Client::Start(const char* url) {
    int screenWidth = 1280;
	int screenHeight = 800;

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Projet IRC - Client");
	SetTargetFPS(144);
	rlImGuiSetup(true);

}

void Client::Update() {

}

void Client::Draw() {
    // Library/rllmGui/example/simple
	
	while (!WindowShouldClose())   
	{
		BeginDrawing();
		ClearBackground(DARKBLUE);

		rlImGuiBegin();

        if (Connexion == 0)
        {
            ImGui::SetCursorPos(ImVec2(50, 100));
            static char strNick[256];
            ImGui::InputTextWithHint("Nicknane", "Input nickname here", strNick, IM_ARRAYSIZE(strNick));

            ImGui::SetCursorPos(ImVec2(50, 135));
            static char strUser[256];
            ImGui::InputTextWithHint("Username", "Input username here", strUser, IM_ARRAYSIZE(strUser));

            ImGui::SetCursorPos(ImVec2(50, 170));
            static int clicked = 0;

            if (ImGui::Button("Connexion", ImVec2(100, 40)))
                clicked++;

            if (clicked & 1)
            {
                bool invalidUsername = false;
                bool invalidNickname = false;
                bool showErrorMessage = false;

                static  std::string errorMessage;

                    if (strlen(strNick) == 0 || strspn(strNick, " \t\n\r\f\v") == strlen(strNick))
                {
                    invalidNickname = true;
                }

                if (strlen(strUser) == 0 || strspn(strUser, " \t\n\r\f\v") == strlen(strUser))
                {
                    invalidUsername = true;
                }

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

                if(showErrorMessage){
                    ImGui::Text(errorMessage.c_str());
                }
                else{
                    ImGui::Text("Connexion");
                }

                
                // Test connexion serveur

                //Connexion = 1;
            }
           
        }
        
        if(Connexion == 1){
            // TODO : Interface Visuelle serveur
        }
		


		rlImGuiEnd();

		EndDrawing();
		
	}
	
	rlImGuiShutdown();
	CloseWindow();        

}

void Client::End() {

}