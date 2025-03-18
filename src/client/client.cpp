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

#define PORT 6667

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
		
		ImGui::SetCursorPos(ImVec2(50,100));
		static char strNick[256];
		ImGui::InputTextWithHint("Nicknane", "Input nickname here", strNick, IM_ARRAYSIZE(strNick));

		ImGui::SetCursorPos(ImVec2(50,135));
		static char strUser[256];
		ImGui::InputTextWithHint("Username", "Input username here", strUser, IM_ARRAYSIZE(strUser));
		
		bool test;
		ImGui::SetCursorPos(ImVec2(50,170));
		static int clicked = 0;
		if(ImGui::Button("Connexion", ImVec2(100, 40)))
		{
			clicked ++;
			test = false;
		}		
		if (clicked)
        {
			ImGui::SetCursorPos(ImVec2(50,225));
			if(test){
				ImGui::Text("Thanks for clicking me!");
			}
			else{
				ImGui::Text("No thanks for clicking me!");
			}
        }


		rlImGuiEnd();

		EndDrawing();
		
	}
	
	rlImGuiShutdown();
	CloseWindow();        

}

void Client::End() {

}