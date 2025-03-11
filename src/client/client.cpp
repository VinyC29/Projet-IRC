#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "client.h"

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "imgui.h"

#define PORT 6667

void Client::Start(const char* url) {
    
}

void Client::Update() {

}

void Client::Draw() {
    // Library/rllmGui/example/simple
    ImGui::Text("Hello, world %d", 123);
    
}

void Client::End() {

}