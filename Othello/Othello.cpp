#include "stdafx.h"

#include <stdio.h>
#include <iostream>
#include <string>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "Game.h"
#include "Jasmin.h"
#include "GUIPlayer.h"
#include "LocalNetworkPlayer.h"
#include "RemoteNetworkPlayer.h"
#include "GUI.h"
#include "Definitions.h"

#pragma comment(lib, "Ws2_32.lib")


using namespace std;

SOCKET setup_server(string port)
{
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		cout << "WSAStartup failed: " << iResult << endl;
		return 1;
	}

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
		return SOCKET_ERROR;
	}

	// Create socket
	SOCKET listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listen_socket == INVALID_SOCKET)
	{
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return SOCKET_ERROR;
	}

	// Bind socket
	iResult = bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		cout << "bind() failed with error: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		closesocket(listen_socket);
		WSACleanup();
		return SOCKET_ERROR;
	}

	freeaddrinfo(result);

	if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "listen() failed with error: " << WSAGetLastError() << endl;
		closesocket(listen_socket);
		WSACleanup();
		return SOCKET_ERROR;
	}

	// Accept incoming connection
	SOCKET client_socket = accept(listen_socket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
	{
		cout << "accept() failed: " << WSAGetLastError() << endl;
		closesocket(listen_socket);
		WSACleanup();
		return SOCKET_ERROR;
	}

	return client_socket;
}

SOCKET setup_client(string ip, string port)
{
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		cout << "WSAStartup failed: " << iResult << endl;
		return 1;
	}

	struct addrinfo *result = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo(ip.c_str(), port.c_str(), &hints, &result);
	if (iResult != 0)
	{
		cout << "getaddrinfo failed: " << iResult << endl;
		WSACleanup();
		return SOCKET_ERROR;
	}

	SOCKET connect_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (connect_socket == INVALID_SOCKET)
	{
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return SOCKET_ERROR;
	}

	// Connect to server
	iResult = connect(connect_socket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		cout << "Unable to connect to server" << endl;
		closesocket(connect_socket);
		WSACleanup();
		return SOCKET_ERROR;
	}

	freeaddrinfo(result);

	return connect_socket;
}

int main(int argc, char* args[])
{
	IPlayer *player1 = NULL, *player2 = NULL;

	cout << "What type of game would you like to play?\n1. Online\n2. Local\n";
	string online_game;
	getline(cin, online_game);

	if (online_game == "1")
	{
		string server_choice, ip, port;
		SOCKET socket;

		cout << "1. Server\n2. Client\n";
		getline(cin, server_choice);
		bool is_server = (server_choice == "1" ? true : false);

		bool connected = false;
		while (!connected)
		{
			if (!is_server)
			{
				cout << "IP: ";
				getline(cin, ip);
			}

			cout << "Port: ";
			getline(cin, port);

			if (is_server)
			{
				socket = setup_server(port);
				if (socket == SOCKET_ERROR)
				{
					continue;
				}
				connected = true;
				IPlayer *localplayer = new GUIPlayer(PLAYER1);
				player1 = new LocalNetworkPlayer(localplayer, socket, PLAYER1);
				player2 = new RemoteNetworkPlayer(socket, PLAYER2);
			}
			else
			{
				socket = setup_client(ip, port);
				if (socket == SOCKET_ERROR)
				{
					continue;
				}
				connected = true;
				IPlayer *localplayer = new GUIPlayer(PLAYER1);
				player1 = new RemoteNetworkPlayer(socket, PLAYER2);
				player2 = new LocalNetworkPlayer(localplayer, socket, PLAYER1);
			}
		}
	}
	else
	{
		player1 = new GUIPlayer(PLAYER1);
		player2 = new Jasmin(PLAYER2);
	}

	Game *game = new Game(player1, player2);
	game->run_game();


	return 0;
}

