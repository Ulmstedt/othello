#pragma once
#include "IPlayer.h"
#include <WinSock2.h>
class RemoteNetworkPlayer :	public IPlayer
{
	SOCKET socket;
public:
	RemoteNetworkPlayer(SOCKET sock, int id);
	~RemoteNetworkPlayer();
	Position play(Board_state);
};

