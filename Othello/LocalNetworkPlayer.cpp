#include "stdafx.h"
#include "LocalNetworkPlayer.h"

using namespace std;

void LocalNetworkPlayer::send_move(Position move)
{
	int bytes_sent = send(socket, (char *)&move, sizeof(Position), 0);
	if (bytes_sent == SOCKET_ERROR)
	{
		cout << "send() failed: " << WSAGetLastError() << endl;
	}
}

LocalNetworkPlayer::LocalNetworkPlayer(IPlayer * p, SOCKET sock, int id)
{
	player_id = id;
	player = p;
	socket = sock;
}

LocalNetworkPlayer::~LocalNetworkPlayer()
{
	closesocket(socket);
	WSACleanup();
	delete(player);
}

Position LocalNetworkPlayer::play(Board_state state)
{
	Position move = player->play(state);
	send_move(move);
	return move;
}
