#include <cstddef>
#include <WinSock2.h>
#include <winsock.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;
#define DEFAULT_PORT "6667"
#define DEFAULT_BUFLEN 2048
#define URL "testnet.ergo.chat"


struct addrinfo *ptr = NULL;
struct addrinfo hints;

int main()
{

    WSADATA wsaData;

    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 1), &wsaData);

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(URL, DEFAULT_PORT, &hints, &ptr);

    SOCKET ConnectSocket = INVALID_SOCKET;

    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                           ptr->ai_protocol);

    // Connect to server.
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

    freeaddrinfo(ptr);

    std::cout << "joining as user" << std::endl;

    int recvbuflen = DEFAULT_BUFLEN;

    char *sendNick = "NICK testtest\r\n";
    char *sendUser = "USER testtest **: Amy Pond\r\n";
    char recvbuf[DEFAULT_BUFLEN];

    // Send an initial buffer
    iResult = send(ConnectSocket, sendNick, strlen(sendNick), 0);
    iResult = send(ConnectSocket, sendUser, strlen(sendUser), 0);
    
    printf("Bytes Sent: %ld\n", iResult);
    
    // Message of the day
    bool motdEnded = false;

    while (!motdEnded)
    {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen - 1, 0);
        if (iResult > 0)
        {
            recvbuf[iResult] = '\0';
            printf("%s", recvbuf);

            // MOTD ending sequence (part of irc protocol)
            if (strstr(recvbuf, " 376 ") || strstr(recvbuf, " 422 "))
            {
                motdEnded = true;
            }
        }
        else if (iResult == 0)
        {
            printf("Connection closed\n");
            return 1;
        }
        else
        {
            printf("recv failed: %d\n", WSAGetLastError());
            return 1;
        }
    }


    // Ajout car doit avoir passer au moins 15 secondes pour voir la liste des channels
    Sleep(15000);
    std::cout << "See channel" << std::endl;
    char *seeChannel = "LIST\r\n";

    bool listEnded = false;

    iResult = send(ConnectSocket, seeChannel, strlen(seeChannel), 0);

    printf("Bytes Sent: %ld\n", iResult);
    while (!listEnded)
    {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen - 1, 0);
        if (iResult > 0)
        {
            recvbuf[iResult] = '\0';
            printf("%s", recvbuf);

            if (strstr(recvbuf, " 323 "))
            {
                listEnded = true;
            }
        }
        else if (iResult == 0)
        {
            printf("Connection closed\n");
            break;
        }
        else
        {
            printf("recv failed: %d\n", WSAGetLastError());
            break;
        }
    }

    std::cout << "joining channel" << std::endl;
    char *joinChannel = "JOIN #test\r\n";

    iResult = send(ConnectSocket, joinChannel, strlen(joinChannel), 0);

    bool joinEnd = false;

    printf("Bytes Sent: %ld\n", iResult);
    while (!joinEnd)
    {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen - 1, 0);
        if (iResult > 0)
        {
            recvbuf[iResult] = '\0';
            printf("%s", recvbuf);
            if (strstr(recvbuf, " 366 "))
            {
                joinEnd = true;
            }
        }
        else if (iResult == 0)
        {
            printf("Connection closed\n");
            break;
        }
        else
        {
            printf("recv failed: %d\n", WSAGetLastError());
            break;
        }
    }

    std::cout << "send a message to the channel" << std::endl;
    char *messageChannel = "PRIVMSG #test :test message\r\n";

    iResult = send(ConnectSocket, messageChannel, strlen(messageChannel), 0);

    if (iResult == SOCKET_ERROR)
    {
        printf("Send failed: %d\n", WSAGetLastError());
    }
    else
    {
        printf("Message sent: %s\n", messageChannel);
    }

    std::cout << "stay in the channel and waiting for message..." << std::endl;
    while (true)
    {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen - 1, 0);
        if (iResult > 0)
        {
            recvbuf[iResult] = '\0';
            printf("%s", recvbuf);

            if (strstr(recvbuf, "PING"))
            {

                char *pongMessage = "PONG";

                send(ConnectSocket, pongMessage, strlen(pongMessage), 0);
                printf("Sent: %s", pongMessage);
            }
        }
        else if (iResult == 0)
        {
            printf("Connection closed\n");
            break;
        }
        else
        {
            printf("recv failed: %d\n", WSAGetLastError());
            break;
        }
    }

    std::cout << "shutdown connexion" << std::endl;
    // shutdown the send half of the connection
    iResult = shutdown(ConnectSocket, SD_SEND);

    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}