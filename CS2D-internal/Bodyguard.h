#pragma once
#include "Player.h"
#include "Utils.h"
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
	void pickTeammateIfNeeded(std::vector<Player*> playersPtrs, uintptr_t moduleBase);

private:
};

