#pragma once
#include "IPlayer.h"
#include <winsock2.h>

class LocalNetworkPlayer : public IPlayer
{
	SOCKET socket = SOCKET_ERROR;
	IPlayer *player = NULL;

	void send_move(Position); // Sends move over socket
public:
	LocalNetworkPlayer(IPlayer *p, SOCKET sock, int id);
	~LocalNetworkPlayer();
	Position play(Board_state);
};

