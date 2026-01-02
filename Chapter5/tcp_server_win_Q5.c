#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
#define STR_SIZE 4
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;

	int clntAdrSz;
	char sendMsg[BUF_SIZE];
	char recvMsg[BUF_SIZE];
	int sendLen = 0, recvLen = 0, strLen = 0;

	if (argc != 2)
	{
		printf("Usage: %s <port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error");
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error");
	}

	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen() error");
	}

	clntAdrSz = sizeof(clntAdr);

	for (int i = 0; i < 5; ++i)
	{
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);
		if (hClntSock == -1)
		{
			ErrorHandling("accept() error");
		}
		else
		{
			printf("Connected client: %d \n", i + 1);
		}

		while ((recvLen = recv(hClntSock, &strLen, STR_SIZE, 0)) != 0)
		{
			int strCnt = 0;
			int idx = 0;
			while (strCnt < strLen)
			{
				recvLen = recv(hClntSock, &recvMsg[idx++], 1, 0);
				if (recvLen == -1)
				{
					ErrorHandling("recv() error");
				}

				strCnt += recvLen;
			}
			recvMsg[idx] = 0;
			printf("Message from client (%d bytes): %s", strCnt, recvMsg);
			
			fputs("Input message: ", stdout);
			fgets(sendMsg, BUF_SIZE, stdin);
			sendLen = strlen(sendMsg);

			send(hClntSock, (char*)&sendLen, sizeof(sendLen), 0);
			send(hClntSock, sendMsg, sendLen, 0);
		}
		closesocket(hClntSock);
		printf("Disconnected client: %d \n", i + 1);
	}
	closesocket(hServSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}