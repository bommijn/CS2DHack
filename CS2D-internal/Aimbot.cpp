#include "Aimbot.h"
#include "Player.h"
#include <math.h>
#include <iostream>

const float PI = 3.14159265358;
const void Aimbot::aimAt(float ourX, float ourY, float enemyX, float enemyY) 
{
	//mem::Nop(nopLoc, bytesToNopSize);
	
	
	std::cout << "ourX = " << ourX << " OurY " << ourY << std::endl;

	
	float targetX = enemyX;
	float targetY = enemyY;
	std::cout << "targetX = " << enemyX << " targetY " << enemyY << std::endl;

	float deltaX = enemyX - ourX;
	float deltaY = enemyY - ourY;
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
	std::cout << "YAW CORRECT  " << yaw << std::cout;
	

	//float radiance = asin((ourX - targetX) / (sqrt(pow(ourX - targetX, 2) + pow(ourY - targetY , 2))));
	
	//std::cout << "Radiance : " << radiance << std::endl;
	//mem::Patch(nopLoc, orignialBytes, bytesToNopSize);
	return void();
}
