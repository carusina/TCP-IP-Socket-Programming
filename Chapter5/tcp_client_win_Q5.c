#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
#define STR_SIZE 4
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAdr;

	char sendMsg[BUF_SIZE];
	char recvMsg[BUF_SIZE];
	int sendLen = 0, recvLen = 0, strLen = 0;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port> \n", argv[0]);
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

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error");
	}
	else
	{
		puts("Connected........");
	}

	while (1)
	{
		fputs("Input message (Q to quit): ", stdout);
		fgets(sendMsg, BUF_SIZE, stdin);

		if (!strcmp(sendMsg, "q\n") || !strcmp(sendMsg, "Q\n"))
		{
			break;
		}

		sendLen = strlen(sendMsg);
		send(hSocket, (char*)&sendLen, sizeof(sendLen), 0);
		send(hSocket, sendMsg, sendLen, 0);

		recvLen = recv(hSocket, &strLen, STR_SIZE, 0);
		if (recvLen == 0)
		{
			break;
		}

		int strCnt = 0;
		int idx = 0;

		while (strCnt < strLen)
		{
			recvLen = recv(hSocket, &recvMsg[idx++], 1, 0);
			if (recvLen == -1)
			{
				ErrorHandling("recv() error");
			}

			strCnt += recvLen;
		}
		recvMsg[idx] = 0;
		printf("Message from server (%d bytes): %s", strCnt, recvMsg);
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