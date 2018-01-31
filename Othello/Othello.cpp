#include "stdafx.h"

#include <stdio.h>
#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "Game.h"
#include "Jasmin.h"
#include "GUIPlayer.h"
#include "GUI.h"
#include "Definitions.h"

#pragma comment(lib, "Ws2_32.lib")


using namespace std;

int main(int argc, char* args[])
{
	bool online_game = true;

	if (online_game)
	{
		cout << "1. Server\n2. Client\nChoice: ";
		char server_choice;
		cin >> server_choice;
		bool is_server = (server_choice == '1' ? true : false);

		string port = "6112";

		WSADATA wsaData;
		int iResult;
		
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0)
		{
			cout << "WSAStartup failed: " << iResult << endl;
			return 1;
		}


		if (is_server)
		{
			struct addrinfo *result = NULL, hints;
		
			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;

			iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
			if (iResult != 0)
			{
				cout << "getaddrinfo failed: " << iResult << endl;
				WSACleanup();
				return 1;
			}

			// Create socket
			SOCKET listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (listen_socket == INVALID_SOCKET)
			{
				cout << "Error at socket(): " << WSAGetLastError() << endl;
				freeaddrinfo(result);
				WSACleanup();
				return 1;
			}

			// Bind socket
			iResult = bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
			if (iResult == SOCKET_ERROR)
			{
				cout << "bind() failed with error: " << WSAGetLastError() << endl;
				freeaddrinfo(result);
				closesocket(listen_socket);
				WSACleanup();
				return 1;
			}

			freeaddrinfo(result);

			if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
			{
				cout << "listen() failed with error: " << WSAGetLastError() << endl;
				closesocket(listen_socket);
				WSACleanup();
				return 1;
			}

			// Accept incoming connection
			SOCKET client_socket = accept(listen_socket, NULL, NULL);
			if (client_socket == INVALID_SOCKET)
			{
				cout << "accept() failed: " << WSAGetLastError() << endl;
				closesocket(listen_socket);
				WSACleanup();
				return 1;
			}

			// TEST TEST TEST
			char recvbuf[512];
			int recvbuflen = 512;

			Position recpos;
			int bytes_to_receive = sizeof(Position);
			int bytes_received = 0;

			cout << "Waiting to recieve" << endl;
			do
			{
				iResult = recv(client_socket, recvbuf, recvbuflen, 0);
				cout << "Bytes recieved: " << iResult << endl;

				memcpy(&recpos + bytes_received, recvbuf, iResult);
				bytes_received += iResult;
			} while (bytes_to_receive - bytes_received > 0);
			
			cout << "Position: (" << recpos.x << ", " << recpos.y << ")" << endl;

			system("PAUSE");
		}
		else // Client
		{
			string ipaddr = "localhost";

			struct addrinfo *result = NULL, hints;

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			iResult = getaddrinfo(ipaddr.c_str(), port.c_str(), &hints, &result);
			if (iResult != 0)
			{
				cout << "getaddrinfo failed: " << iResult << endl;
				WSACleanup();
				return 1;
			}

			SOCKET connect_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (connect_socket == INVALID_SOCKET)
			{
				cout << "Error at socket(): " << WSAGetLastError() << endl;
				freeaddrinfo(result);
				WSACleanup();
				return 1;
			}

			// Connect to server
			iResult = connect(connect_socket, result->ai_addr, (int)result->ai_addrlen);
			if (iResult == SOCKET_ERROR)
			{
				cout << "Unable to connect to server" << endl;
				system("PAUSE");
				closesocket(connect_socket);
				WSACleanup();
				return 1;
			}

			freeaddrinfo(result);

			// TEST TEST TEST
			char *sendbuf = "this is a test";
			Position sendpos = { 3, 7 };

			cout << "Sending data" << endl;
			iResult = send(connect_socket, (char *)&sendpos, sizeof(Position), 0);
			if (iResult == SOCKET_ERROR)
			{
				cout << "send() failed: " << WSAGetLastError() << endl;
				closesocket(connect_socket);
				WSACleanup();
				return 1;
			}

			cout << "Bytes sent: " << iResult << endl;
			system("PAUSE");
		}
		
	}
	else
	{
		IPlayer *player1 = new GUIPlayer(PLAYER1);
		IPlayer *player2 = new Jasmin(PLAYER2);
		Game *game = new Game(player1, player2);
		game->run_game();
	}
	

    return 0;
}

