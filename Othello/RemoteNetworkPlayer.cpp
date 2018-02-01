#include "stdafx.h"
#include "RemoteNetworkPlayer.h"
#include "Game.h"

using namespace std;

RemoteNetworkPlayer::RemoteNetworkPlayer(SOCKET sock, int id)
{
	player_id = id;
	socket = sock;
}


RemoteNetworkPlayer::~RemoteNetworkPlayer()
{
	closesocket(socket);
	WSACleanup();
}

Position RemoteNetworkPlayer::play(Board_state)
{
	Position move;
	int bytes_left = sizeof(Position);
	char *recvptr = (char*)&move;

	while (bytes_left > 0)
	{
		int bytes_read = recv(socket, recvptr, bytes_left, 0);
		if (bytes_read < 0)
		{
			Game::quit_game();
		}
		recvptr += bytes_read;
		bytes_left -= bytes_read;
	}

	return move;
}
