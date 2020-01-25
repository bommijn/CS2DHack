#include "Bodyguard.h"

// void SendKeyInput(HWND &wHANDLE, WPARAM key)
// {
// 	PostMessage(wHANDLE, WM_KEYDOWN, key, 0);
// 	Sleep(50);
// 	PostMessage(wHANDLE, WM_KEYUP, key, 0);
// }


void pressKey(HWND &wHANDLE, WPARAM key)
{
	PostMessage(wHANDLE, WM_KEYDOWN, key, 0);
	Sleep(10);
}


void releaseKey(HWND &wHANDLE, WPARAM key)
{
	PostMessage(wHANDLE, WM_KEYUP, key, 0);
	Sleep(10);
}


void Bodyguard::run(HWND topWindow, std::vector<Player*> playersPtrs, Player* localPlayerPtr ) {
	ourPlayer = localPlayerPtr;
	std::cout << "reunning bodyguard!" << followingPlayerPtr << "\n";
	

	if (followingPlayerPtr != NULL && followingPlayerPtr->placeholderForVtable == ourPlayer->placeholderForVtable )
	{
		std::cout << "Picked player: " << followingPlayerPtr << "\n";

		if (ourPlayer->xCoord == followingPlayerPtr->xCoord )
		{
			std::cout<< "standing horizontally! \n";
			releaseKey(topWindow, 0x41);
			releaseKey(topWindow, 0x44);
		} else if (followingPlayerPtr->xCoord > ourPlayer->xCoord ) {
			std::cout<< "MOVING RIGHT! \n";
			pressKey(topWindow, 0x44);
			releaseKey(topWindow, 0x41);
			// SendKeyInput(topWindow, 0x44);
			// ourPlayer->xCoord = followingPlayerPtr->xCoord;
		} else {
			std::cout<< "MOVING left! \n";
			pressKey(topWindow, 0x41);
			releaseKey(topWindow, 0x44);
			// SendKeyInput(topWindow, 0x41);
			// ourPlayer->xCoord = followingPlayerPtr->xCoord;
		}
		if (abs(ourPlayer->xCoord - followingPlayerPtr->xCoord) > 45 )
		{
			ourPlayer->xCoord = followingPlayerPtr->xCoord;
		}

		if (ourPlayer->yCoord == followingPlayerPtr->yCoord )
		{
			std::cout<< "standing vertically! \n";
		} else if (followingPlayerPtr->yCoord > ourPlayer->yCoord ) {
			pressKey(topWindow, 0x53);
			releaseKey(topWindow, 0x57);
			// SendKeyInput(topWindow, 0x53);
			std::cout<< "MOVING down! \n";
			// ourPlayer->yCoord = followingPlayerPtr->yCoord;
		} else {
			pressKey(topWindow, 0x57);
			releaseKey(topWindow, 0x53);
			// SendKeyInput(topWindow, 0x57);
			std::cout<< "MOVING up! \n";
			// ourPlayer->yCoord = followingPlayerPtr->yCoord;
		}	
		if (abs(ourPlayer->yCoord - followingPlayerPtr->yCoord) > 45 )
		{
			ourPlayer->yCoord = followingPlayerPtr->yCoord;
		}


	}
	
}


void Bodyguard::pickTeammateIfNeeded(std::vector<Player*> playersPtrs, uintptr_t moduleBase) {
	if (followingPlayerPtr == NULL || followingPlayerPtr->placeholderForVtable != ourPlayer->placeholderForVtable || followingPlayerPtr->team != ourPlayer->team)
	{
		for (auto playerIt = playersPtrs.begin(); playerIt != playersPtrs.end(); ++playerIt) {

			if ((*playerIt)->team == ourPlayer->team && (*playerIt) != ourPlayer && !(*playerIt)->is_dead() )
			{
				followingPlayerPtr = *playerIt;
				wchar_t myarray[51] = L"any string here is fiiiine :D"; 
				Utils::SendChatMessageToAll(moduleBase,(uintptr_t)&myarray);

			}
	};

	};

 
};