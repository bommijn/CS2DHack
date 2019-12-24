#include "Aimbot.h"
#include "Player.h"
#include <math.h>
#include <iostream>

const float PI = 3.14159265358;
const void Aimbot::aimAt(Player* localPlayerPtr, Player* enemyPtr) 
{
	mem::Nop(nopLoc, bytesToNopSize);
	
	
	std::cout << "localPlayerPtr->xCoord = " << localPlayerPtr->xCoord << " localPlayerPtr->yCoord " << localPlayerPtr->yCoord << std::endl;

	
	float targetX = enemyPtr->xCoord;
	float targetY = enemyPtr->yCoord;
	std::cout << "targetX = " << enemyPtr->xCoord << " targetY " << enemyPtr->yCoord << std::endl;

	float deltaX = enemyPtr->xCoord - localPlayerPtr->xCoord;
	float deltaY = enemyPtr->yCoord - localPlayerPtr->yCoord;
	float yaw = atan2(deltaX, deltaY);

	yaw = (yaw / PI * 180);
	yaw = yaw - 180;
	if (yaw < 0)
	{
		yaw = -yaw;
	}
	
	int remainder = 0 ;
	if (yaw > 270)
	{
		remainder = (int)yaw % 270;
		if (remainder > 0)
		{
			remainder = 90 - remainder;
			yaw = remainder * -1;
		}
	}
	std::cout << "YAW CORRECT  " << yaw << std::endl;

	localPlayerPtr->viewAngleX = yaw;

	return void();
}
