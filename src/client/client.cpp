#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include "imgui.h"
#include "client.h" 
#include "connectIRC.h"
#define PORT "6667"
#define URL "irc.libera.chat"

static  std::string connexionMessage = "";

// testnet.ergo doit attendre 15 sec pour avoir la liste des channels
bool haveChannel = false;
bool listChannel = false;

Client::Client(float w,float h) : m_Width(w), m_Height(h)  {
    
}

void Client::Start(bool secure, const char* url) {
    bool m_Secure = secure;

    setConnexionState(AWAITING_CONNEXION);

}

void Client::Update() {
    
    if (m_connexionState == SENDING_CONNEXION_INFO_TO_SERVER)
    {
        WSAStartup(DllVersion, &wsaData);
        socket = ConnectIRC::CreateSocket();
        ConnectIRC::Connect(&socket, false, URL, false);
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

    if(m_connexionState == CONNECTED_TO_SERVER && !listChannel){
        Sleep(15000);
        char seeChannel[256] = {0};
        snprintf(seeChannel, 256, "LIST\r\n");
        ConnectIRC::SendMsg(&socket, seeChannel);
        listChannel = true;
    }

    if(m_connexionState == JOINNIG_CHANNEL){
        if (!m_FirstJoin)
        {
            char partChannel[256] = {0};
            snprintf(partChannel, 256, "PART %s\r\n", strOldChannel);
            ConnectIRC::SendMsg(&socket, partChannel);
        }

        char joinChannel[256] = {0};
        char chatHistory[256] = {0};
        snprintf(joinChannel, 256, "JOIN %s\r\n", strChannel);

        ConnectIRC::SendMsg(&socket, joinChannel);
        m_connexionState = CONNECTED_TO_SERVER;
        
        if(m_FirstJoin){
            m_FirstJoin = false;
        }
    }

    if(m_SendingMsg && !m_FirstJoin){
        char msg[512] = {0};
        snprintf(msg, 512, "PRIVMSG %s :%s\r\n", strChannel, strMsg);
        std::cout << msg << std::endl;
        ConnectIRC::SendMsg(&socket, msg);
        m_SendingMsg = false;

        if(chatHistoryCount < 256){
            char message [512];
            snprintf(message, 512, "<%s> %s", strNick, strMsg);
            chatHistory[chatHistoryCount] = _strdup(message);
            chatHistoryCount++;
        }
    }

    char **parsedResponse;
    parsedResponse = ConnectIRC::ReceiveMsg(&socket, "\r\n ");

    if (parsedResponse != nullptr) {
        if(m_connexionState == AWAIT_SERVER_ANSWER_TO_CONNEXION ){
            char **ptrCode = parsedResponse;
            while (*ptrCode != nullptr) {
                if (strcmp(*ptrCode, "001") == 0) {
                    setConnexionState(CONNECTED_TO_SERVER);
                }
                ++ptrCode;
            }       
        }
        else if (m_connexionState == AWAIT_SERVER_ANSWER_TO_CONNEXION && strcmp(parsedResponse[1], "001") != 0) {
            setConnexionState(AWAITING_CONNEXION);
        }

        if(strcmp(parsedResponse[0], "PING") == 0){
            char pingAnswer[256] = {0};
            snprintf(pingAnswer, 256, "PONG :%s\r\n", parsedResponse[1]);
            ConnectIRC::SendMsg(&socket, pingAnswer);
        }


        char **ptrCode = parsedResponse;

            while (*ptrCode != nullptr) {
                if (strcmp(*ptrCode, "322") == 0)
                {
                    char *channelToken = *(ptrCode + 2);
                    if(channelToken != nullptr){
                        channels.push_back(channelToken);
                        channelCount++;
                    }  
                    haveChannel = true;
                }

                if (strcmp(*ptrCode, "353") == 0)
                {
                    for (int i = 0; i < channelUsers; i++)
                    {
                        free(userChannels[i]);
                        userChannels[i] = nullptr;
                    }
                    channelUsers = 0;

                    char **userPtr = ptrCode + 5;
                    while (*userPtr != nullptr)
                    {

                        if (*userPtr != nullptr && strcmp(*userPtr, "366") == 0)
                        {
                            break;
                        }
                        
                        char *token = *userPtr;
                        if (token[0] == ':')
                            token++;

                        userChannels[channelUsers++] = _strdup(token);
                        userPtr++;
                    }
                }

                ++ptrCode;
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
    else if(m_connexionState == CONNECTED_TO_SERVER || m_connexionState == JOINNIG_CHANNEL){

        static int item_selected_idx = 0; // Here we store our selected data as an index.
        static bool item_highlight = true;
        int item_highlighted_idx = -1; // Here we store our highlighted data as an index.

        if(haveChannel){
            ImGui::Text("Available channels");
            if (ImGui::BeginListBox("##listbox channels", ImVec2(200, 25 * ImGui::GetTextLineHeightWithSpacing())))
            {
                for (int n = 0; n < channelCount; n++)
                {
                    bool is_selected = (item_selected_idx == n);
                    ImGuiSelectableFlags flags = (item_highlighted_idx == n) ? ImGuiSelectableFlags_Highlight : 0;
                    if (ImGui::Selectable(channels[n].c_str(), is_selected, flags))
                    {
                        if (item_selected_idx != n)
                        {
                            item_selected_idx = n;
                            if(!m_FirstJoin){
                                strcpy(strOldChannel, strChannel);
                            }
                            strcpy(strChannel, channels[item_selected_idx].c_str());
                            m_connexionState = JOINNIG_CHANNEL;
                        }
                    }
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
        }

        ImGui::SetCursorPos(ImVec2(225, 50));
        ImGui::BeginChild("ChatMessages", ImVec2(825, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
        for (int i = 0; i < chatHistoryCount; ++i)
        {
            if (chatHistory[i])
            {
                ImGui::TextWrapped("%s", chatHistory[i]);
            }
        }
        ImGui::EndChild();

        ImGui::SetCursorPos(ImVec2(225, 375));
        strMsg[256];
        ImGui::InputTextMultiline("##ChatInput", strMsg, IM_ARRAYSIZE(strMsg), ImVec2(600, 100));

        static int clicked = 0;
        static bool showConnexionMessage = false;
        
        ImGui::SetCursorPos(ImVec2(850, 375));
        if (ImGui::Button("Send Message", ImVec2(200, 100)))
        {
            clicked++;
            m_SendingMsg = true;
        }

        if (!m_FirstJoin)
        {
            ImGui::SetCursorPos(ImVec2(1075, 50));
            if (ImGui::BeginListBox("##listbox users", ImVec2(200, 25 * ImGui::GetTextLineHeightWithSpacing())))
            {
                for (int n = 0; n < channelUsers; n++)
                {
                    bool is_selected1 = (item_selected_idx == n);
                    ImGuiSelectableFlags flags = (item_highlighted_idx == n) ? ImGuiSelectableFlags_Highlight : 0;
                    if (ImGui::Selectable(userChannels[n], is_selected1, flags))
                    {
                        if (item_selected_idx != n)
                        {
                            item_selected_idx = n;
                        }
                    }
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected1)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
        }
    }

}

void Client::End() {
    ConnectIRC::Shutdown();
}

void Client::setConnexionState(ClientState state){
    m_connexionState = state;
}