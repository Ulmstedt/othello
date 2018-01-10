#pragma once
#include "IPlayer.h"
#include <vector>
#
class Jasmin : public IPlayer
{
public:
	Jasmin(int id);
	~Jasmin();

	Position play(Board_state) override;
	std::vector<Position> get_flips(Board_state board, Position pos, int player) const;
	std::vector<Position> Jasmin::check_dir(Board_state board, Position pos, int xdir, int ydir, int player) const;
	bool inside_board(Position position) const;
};

