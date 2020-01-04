#pragma once
#include "Player.h"
#include <vector>
#include <iostream>

class Bodyguard
{
public:
	Player* ourPlayer;
	Player* followingPlayerPtr;
	float lastFollowingPlayerXCoord;
	float lastFollowingPlayerYCoord;

	void run(HWND topWindow, std::vector<Player*> playersPtrs, Player* localPlayerPtr );
private:
	void pickTeammate(std::vector<Player*> playersPtrs);
};

