#include "Bodyguard.h"

void Bodyguard::run(std::vector<Player*> playersPtrs, Player* localPlayerPtr ) {
	ourPlayer = localPlayerPtr;
	std::cout << "reunning bodyguard!" << followingPlayerPtr << "\n";
	if (followingPlayerPtr == NULL || followingPlayerPtr->placeholderForVtable != ourPlayer->placeholderForVtable)
	{
		pickTeammate(playersPtrs);
	};
	if (followingPlayerPtr != NULL && followingPlayerPtr->placeholderForVtable == ourPlayer->placeholderForVtable )
	{
		std::cout << "Picked player: " << followingPlayerPtr << "\n";

		if (lastFollowingPlayerXCoord == followingPlayerPtr->xCoord )
		{
			std::cout<< "standing horizontally! \n";
		} else if (followingPlayerPtr->xCoord > lastFollowingPlayerXCoord ) {
			std::cout<< "MOVING RIGHT! \n";
			ourPlayer->xCoord = followingPlayerPtr->xCoord + 2.0f;
		} else {
			std::cout<< "MOVING left! \n";
			ourPlayer->xCoord = followingPlayerPtr->xCoord - 2.0f;
		}

		if (lastFollowingPlayerYCoord == followingPlayerPtr->yCoord )
		{
			std::cout<< "standing vertically! \n";
		} else if (followingPlayerPtr->yCoord > lastFollowingPlayerYCoord ) {
			std::cout<< "MOVING down! \n";
			ourPlayer->yCoord = followingPlayerPtr->yCoord + 2.0f;
		} else {
			std::cout<< "MOVING up! \n";
			ourPlayer->yCoord = followingPlayerPtr->yCoord - 2.0f;
		}	

	}
	
}


void Bodyguard::pickTeammate(std::vector<Player*> playersPtrs) {
	for (auto playerIt = playersPtrs.begin(); playerIt != playersPtrs.end(); ++playerIt) {

		if ((*playerIt)->team == ourPlayer->team && (*playerIt) != ourPlayer )
		{
			followingPlayerPtr = *playerIt;
		}
	};
 
};