#include "Aimbot.h"
#include "Player.h"
#include <math.h>
#include <iostream>
#include <iomanip>

const float PI = 3.14159265358;

Player* Aimbot::getClosestEnemy(std::vector<Player*> playersPtrs, Player* localPlayerPtr ) {
	float closestDitance = 10000000;
	Player *closestPlayer = 0;

	for (auto possibleEnemy = playersPtrs.begin(); possibleEnemy != playersPtrs.end(); ++possibleEnemy) {

		// here we should instantiate the player class exported from RECLASS whenever we have one

		std::cout << "DSAD dJSADkJDSAkjHJ a vida aki!:" << std::dec << (*possibleEnemy)->get_health() << "\n";

		float thisEnemyDistance = (float)sqrt(
        pow(double((*possibleEnemy)->xCoord - localPlayerPtr->xCoord), 2.0) +
        pow(double((*possibleEnemy)->yCoord - localPlayerPtr->yCoord), 2.0));

		if ((*possibleEnemy)->xCoord > 0 && (*possibleEnemy)->yCoord > 0 && (*possibleEnemy)->team != localPlayerPtr->team && thisEnemyDistance < closestDitance && !(*possibleEnemy)->is_dead() && !localPlayerPtr->is_dead())
		{
			closestDitance = thisEnemyDistance;
			closestPlayer = (*possibleEnemy);
		}

	};

	// std::cout << "closestDitance: " << std::fixed << std::setprecision(3) << closestDitance << std::endl;
	// std::cout << "closestPlayer Position X :" << closestPlayer->xCoord << "\n";
	// std::cout << "closestPlayer Position Y :" << closestPlayer->yCoord << "\n";
	
	return closestPlayer;
}


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
