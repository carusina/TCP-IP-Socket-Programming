#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	char sendMessage[30];
	int strLen = 0;
	int idx = 0, readLen = 0;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error");
	}

	while (1)
	{
		readLen = recv(hSocket, message, sizeof(message), 0);
		if (readLen == -1)
		{
			ErrorHandling("read() error");
		}

		printf("Message from Server: %s \n", message);

		printf("Input Message send to Server: ");
		fgets(sendMessage, sizeof(sendMessage), stdin);
		int len = strlen(sendMessage);
		if (len > 0 && sendMessage[len - 1] == '\n')
		{
			sendMessage[len - 1] = '\0';
		}

		send(hSocket, sendMessage, sizeof(sendMessage), 0);
		if (strcmp(sendMessage, "Q") == 0)
		{
			break;
		}
		sendMessage[0] = '\0';
	}

	closesocket(hSocket);
	WSACleanup();

	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}