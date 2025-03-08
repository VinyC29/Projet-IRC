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

    do
    {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
        {
            printf("Bytes received: %d\n", iResult);
            printf("Message: %s\n", recvbuf);
        }
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);

    std::cout << "joining as user" << std::endl;
    char *joinChannel = "JOIN #chat\r\n";

    iResult = send(ConnectSocket, joinChannel, strlen(joinChannel), 0);

    printf("Bytes Sent: %ld\n", iResult);
    do
    {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
        {
            printf("Bytes received: %d\n", iResult);
            printf("Message: %s\n", recvbuf);
        }
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);

    // shutdown the send half of the connection
    iResult = shutdown(ConnectSocket, SD_SEND);

    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}