#pragma once
#include "mem.h"
#include "Player.h"
class Aimbot
{
private:
	int bytesToNopSize = 6;
	BYTE* orignialBytes{ (BYTE*)"\xD9\x9B\xDC\x01\x00\x00" };
	BYTE* nopLoc = (BYTE*)0x6387BA;	// adress where bullet angle gets set


public:
	const void aimAt(float ourX, float ourY, float enemyX, float enemyY);

};

