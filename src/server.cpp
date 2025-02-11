#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define PORT 6697

int main() {
	// Init WINSOCK
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);

	WSAStartup(DllVersion, &wsaData);

	// Create Socket
    int iFamily = AF_INET;
    int iType = SOCK_STREAM;
    int iProtocol = IPPROTO_TCP;

	SOCKET sock = socket(iFamily, iType, iProtocol);

	// Define server info
	SOCKADDR_IN sin;
	ZeroMemory(&sin, sizeof(sin));
	sin.sin_port = htons(PORT);
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Connect to server
	if (connect(sock, (const sockaddr*)&sin, sizeof(sin)) != 0) {
		ExitProcess(EXIT_FAILURE);
	}

/* 		TODO: this is currently a client and it is connecting to a server, this is supposed to be a server, 
		so change this connection to a "waiting for client" thing. Then do the IRC protocol inputs and outputs.		*/

	const char szMsg[] = "HEAD / HTTP/1.0\r\n\r\n";
	if (!send(sock, szMsg, strlen(szMsg), 0)) {
		ExitProcess(EXIT_FAILURE);
	}

	char szBuffer[4096];
	char szTemp[4096];

	for (int i = 0; i < sizeof(szBuffer); i++) { 
		szBuffer[i] = '\0'; 
	}

	while (recv(sock, szTemp, 4096, 0)) {
		strcat(szBuffer, szTemp);
	}

	printf("%s\n", szBuffer);

	closesocket(sock);
	getchar();

	ExitProcess(EXIT_SUCCESS);
}