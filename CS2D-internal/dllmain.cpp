#include "pch.h"
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <psapi.h>
#include <algorithm>
#include "Player.h"
#include "PlayerLinkedList.h"
#include "mem.h"
#include "Aimbot.h"
#include "Utils.h"
#include "Bodyguard.h"
#include <cwchar>

struct Vector2f
{
	float x = 0;
	float y = 0;

	Vector2f() {

	}
	Vector2f(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2f operator-(Vector2f s)
	{
		return Vector2f(this->x - s.x, this->y - s.y);
	}

	Vector2f operator+(Vector2f s)
	{
		return Vector2f(this->x + s.x, this->y + s.y);
	}


	Vector2f operator*(int a)
	{
		return Vector2f(this->x * a, this->y * a);
	}

	float Magnitude()
	{
		return sqrtf(pow(this->x, 2) + pow(this->y, 2));
	}

	Vector2f Normalize()
	{
		float mag = this->Magnitude();
		return Vector2f(this->x / mag, this->y / mag);
	}
};

struct Vector2i
{
	int x = 0;
	int y = 0;
	Vector2i()
	{

	}
	Vector2i(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2i operator+(Vector2f s)
	{
		return Vector2i((int)(this->x + s.x), (int)(this->y + s.y));
	}

	Vector2i operator+(Vector2i s)
	{
		return Vector2i(this->x + s.x, this->y + s.y);
	}
};

typedef int(__cdecl* fnPosToCell)(double a1);
fnPosToCell PosToCell;


uintptr_t localPlayerLinkedListOffset = 0x0486330;
Aimbot aimbot;
Bodyguard bodyguard;
std::vector<uintptr_t> realPlayerBases;
std::vector<uintptr_t> localPlayerLinkedListPointers = { 0x8, 0xc, 0x0 };
std::vector<uintptr_t> timeTotalRoundMinutesOffsets = { 0x1c };

std::vector<uintptr_t> filterRealPlayerBases(std::vector<uintptr_t> possiblePlayerBases, uintptr_t knownLocalPlayerVtable) {

	// std::cout << "AEWS knownLocalPlayerVtable :" << std::hex << knownLocalPlayerVtable << "\n";


	auto it = std::remove_if(possiblePlayerBases.begin(), possiblePlayerBases.end(), [&knownLocalPlayerVtable](uintptr_t playerBase) {
		return *(uintptr_t*)playerBase != knownLocalPlayerVtable;
		});
	possiblePlayerBases.erase(it, possiblePlayerBases.end());

	return possiblePlayerBases;
}

std::vector<uintptr_t> getPossiblePlayersPointers(uintptr_t moduleBase) {
	uintptr_t address = mem::FindDMAAddy(moduleBase + localPlayerLinkedListOffset, localPlayerLinkedListPointers);
	// std::cout << "linked list address :" << std::hex << address << "\n";

	PlayerLinkedList* playerLinkedList = (PlayerLinkedList*)(address);
	// std::cout << "linked list playerBasePointer :" << std::hex << playerLinkedList->playerBasePointer << "\n";
	// std::cout << "linked list nextOnList :" << std::hex << playerLinkedList->nextOnList << "\n";
	// std::cout << "linked list previousOnList :" << std::hex << playerLinkedList->previousOnList << "\n";

	std::vector<PlayerLinkedList> linkedListMembers = { *playerLinkedList };
	std::vector<uintptr_t> possiblePlayerBases = { playerLinkedList->playerBasePointer };

	for (int i = 0; i < 1000; ++i)
	{

		PlayerLinkedList* newLinkedList = (PlayerLinkedList*)(linkedListMembers.back().nextOnList);
		// std::cout << "newLinkedList playerBasePointer :" << std::hex << newLinkedList->playerBasePointer << "\n";
		// std::cout << "newLinkedList nextOnList :" << std::hex << newLinkedList->nextOnList << "\n";
		// std::cout << "newLinkedList previousOnList :" << std::hex << newLinkedList->previousOnList << "\n";


		if (std::find(possiblePlayerBases.begin(), possiblePlayerBases.end(), newLinkedList->playerBasePointer) != possiblePlayerBases.end()) {
			break;
		}
		else {
			possiblePlayerBases.push_back(newLinkedList->playerBasePointer);
			linkedListMembers.push_back(*newLinkedList);
		}

	}

	return possiblePlayerBases;
}


HWND FindTopWindow(DWORD pid)
{
    std::pair<HWND, DWORD> params = { 0, pid };

    // Enumerate the windows using a lambda to process each window
    BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL 
    {
        auto pParams = (std::pair<HWND, DWORD>*)(lParam);

        DWORD processId;
        if (GetWindowThreadProcessId(hwnd, &processId) && processId == pParams->second)
        {
            // Stop enumerating
            SetLastError(-1);
            pParams->first = hwnd;
            return FALSE;
        }

        // Continue enumerating
        return TRUE;
    }, (LPARAM)&params);

    if (!bResult && GetLastError() == -1 && params.first)
    {
        return params.first;
    }

    return 0;
}

DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"cs2d.exe");		
	Player* localPlayerPtr = (Player*)*(uintptr_t*)(moduleBase + 0x0496E0C);
	
	uintptr_t *timeMinutesLeftPtr = (uintptr_t*)(moduleBase + 0x496F18); 
	uintptr_t *timeSecondsLeftPtr = (uintptr_t*)(moduleBase + 0x496F1C); 
	PosToCell = (fnPosToCell)(moduleBase + 0x3B5F30);



	uintptr_t timeTotalRoundMinutesAddress = mem::FindDMAAddy(moduleBase + 0x497444, timeTotalRoundMinutesOffsets);
	uintptr_t *timeTotalRoundMinutesPtr = (uintptr_t*)timeTotalRoundMinutesAddress;
	bool isBegginingOfRound = false;

	while (true)
	{
			   		 
		HWND topWindow = FindTopWindow(GetCurrentProcessId());

		
		// std::cout << "\n *timeTotalRoundMinutesPtr: " << *timeTotalRoundMinutesPtr <<  '\n';
		// std::cout << "\n *timeMinutesLeftPtr: " << *timeMinutesLeftPtr <<  '\n';
		// std::cout << "\n *timeSecondsLeftPtr: " << *timeSecondsLeftPtr <<  '\n';

		// std::cout << "localPlayerPtr :" << std::hex << *localPlayerPtr << "\n";

		if (GetAsyncKeyState(0x30) & 1)
		{
			break;
		}

		
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			// std::cout << "localPlayerPtr base value:" << std::hex << *(uintptr_t*)localPlayerPtr << "\n";
			// float shootDirection = *(float*)((*localPlayerPtr)+0x1DC );

			//std::cout << "Direction in which the bullet will fly = " << std::fixed << std::setprecision(3) << localPlayerPtr->viewAngleOne << std::endl;
		}

		if (GetAsyncKeyState(VK_NUMPAD4) & 1)
		{
		}

		std::vector<uintptr_t> possiblePlayerBases = getPossiblePlayersPointers(moduleBase);

		realPlayerBases = filterRealPlayerBases(possiblePlayerBases, (uintptr_t)(*((uintptr_t*)(localPlayerPtr))));
		std::vector<Player*> playersPtrs;
		for (auto playerBasePtr = realPlayerBases.begin(); playerBasePtr != realPlayerBases.end(); ++playerBasePtr) {
			// here we should instantiate the player class exported from RECLASS whenever we have one

			Player* player = (Player*)(*playerBasePtr);
			// std::cout << (uintptr_t)player << std::endl;
			// std::cout << "Player Base :" << (uintptr_t)(player) << "\n";
			// std::cout << "Player Position X :" << player->xCoord << "\n";
			// std::cout << "Player Position Y :" << player->yCoord << "\n";
			// std::cout << "---------------------- \n";
			playersPtrs.push_back(player);
		};


		if (!isBegginingOfRound && *timeMinutesLeftPtr == *timeTotalRoundMinutesPtr && *timeSecondsLeftPtr == 0)
		{
			isBegginingOfRound = true;
			std::cout << "Round started! Put round started logic below here. Will run once. \n";
			bodyguard.pickTeammateIfNeeded(playersPtrs, moduleBase);
		}

		if (isBegginingOfRound && !(*timeMinutesLeftPtr == *timeTotalRoundMinutesPtr && *timeSecondsLeftPtr == 0))
		{
			isBegginingOfRound = false;
			std::cout << "Round ain't starting anymore :p \n";
		}


		if (GetAsyncKeyState(0x39) & 1)
		{
			DWORD mapObj = 0x497E4C;
			DWORD tileCount = 0x14;
			DWORD mapYCount = 0x18;
			DWORD first_TileType = 0x1C;

			DWORD mapPtr = *(DWORD*)(moduleBase + mapObj);
			int tileYCount = *(int*)(mapPtr + mapYCount);
			DWORD firstTileAddress = mapPtr + first_TileType;
			DWORD tilePixelSize = 0x4CF274;

	   	      std::cout << "mapPtr: " << std::hex  << mapPtr << "\n";
	   	      std::cout << "tileYCount: " << std::dec  << tileYCount << "\n";
	   	      std::cout << "firstTileAddress: " << std::hex  << firstTileAddress << "\n";

	   	    std::cout << "Player Position X :" << localPlayerPtr->xCoord << "\n";
			std::cout << "Player Position Y :" << localPlayerPtr->yCoord << "\n";

			Player *enemy = 0;

			for (auto possibleEnemy = playersPtrs.begin(); possibleEnemy != playersPtrs.end(); ++possibleEnemy) {

				if ((*possibleEnemy)->team != localPlayerPtr->team )
				{
					enemy = (*possibleEnemy);
				}

			};

	   	    std::cout << "enemy Position X :" << enemy->xCoord << "\n";
			std::cout << "enemy Position Y :" << enemy->yCoord << "\n";

			float maxLen = 340282346638528859811704183484516925440.0000000000000000f; //max distance 
			int targetIndex = -1;

			Vector2f localPlayerPos = Vector2f(localPlayerPtr->xCoord, localPlayerPtr->yCoord);
			Vector2f enemyPos = Vector2f(enemy->xCoord, enemy->yCoord);

			Vector2f dist = localPlayerPos - enemyPos;
			if (dist.x < 0) dist.x *= -1; if (dist.y < 0) dist.y *= -1; //abs

			float len = dist.Magnitude();
			Vector2f dist_n = enemyPos - localPlayerPos;


	   	    std::cout << "dist X :" << dist.x << "\n";
	   	    std::cout << "dist y :" << dist.y << "\n";
			std::cout << "len :" << len << "\n";
	   	    std::cout << "dist_n x:" << dist_n.x << "\n";
	   	    std::cout << "dist_n y :" << dist_n.y << "\n";

	   	    Vector2f direction = Vector2f(dist_n.x / len, dist_n.y / len);
			direction = direction * 20;
			Vector2i temp_pos = Vector2i((int)localPlayerPos.x, (int)localPlayerPos.y);
			bool wallFound = false;

			std::cout << "direction y :" << direction.y << "\n";
			std::cout << "direction x :" << direction.x << "\n";

			if (direction.x > 0)
				{
					if (direction.y > 0)
					{
						while (temp_pos.x <= enemyPos.x || temp_pos.y <= enemyPos.y)
						{
							//std::cout << "dusmanin sol ustundeyim" << std::endl;
							temp_pos = temp_pos + direction;
							//std::cout << temp_pos.x << std::endl;
							//std::cout << temp_pos.y << std::endl;
							float div = *(float*)(moduleBase + tilePixelSize);
							int x = PosToCell((double)((temp_pos.x) / div));
							int y = PosToCell((double)((temp_pos.y) / div));
							//std::cout << x << std::endl;
							//std::cout << y << std::endl;
							int type = *(int*)(firstTileAddress + 4 * ((x * tileYCount) + y));
							//std::cout << std::hex << type << std::endl;
							//std::cout << std::hex << (firstTileAddress + 4 * ((x * tileYCount) + y)) << std::endl;
							if (type == 1 || type == 3)
							{
								wallFound = true;
								break;
							}
						}
					}
					else
					{
						while (temp_pos.x < enemyPos.x || temp_pos.y > enemyPos.y)
						{
							//std::cout << "dusmanin sol altindayim" << std::endl;
							temp_pos = temp_pos + direction;
							//std::cout << temp_pos.x << std::endl;
							//std::cout << temp_pos.y << std::endl;
							float div = *(float*)(moduleBase + tilePixelSize);
							int x = PosToCell((double)((temp_pos.x) / div));
							int y = PosToCell((double)((temp_pos.y) / div));
							//std::cout << x << std::endl;
							//std::cout << y << std::endl;
							int type = *(int*)(firstTileAddress + 4 * ((x * tileYCount) + y));
							//std::cout << std::hex << type << std::endl;
							//std::cout << std::hex << (firstTileAddress + 4 * ((x * tileYCount) + y)) << std::endl;
							if (type == 1 || type == 3)
							{
								wallFound = true;
								break;
							}
						}

					}
				}
				else
				{
					if (direction.y > 0)
					{
						while (temp_pos.x >= enemyPos.x || temp_pos.y <= enemyPos.y)
						{
							//std::cout << "dusmanin sag ustundeyim" << std::endl;
							temp_pos = temp_pos + direction;
							//std::cout << temp_pos.x << std::endl;
							//std::cout << temp_pos.y << std::endl;
							float div = *(float*)(moduleBase + tilePixelSize);
							int x = PosToCell((double)((temp_pos.x) / div));
							int y = PosToCell((double)((temp_pos.y) / div));
							//std::cout << x << std::endl;
							//std::cout << y << std::endl;
							int type = *(int*)(firstTileAddress + 4 * ((x * tileYCount) + y));
							//std::cout << std::hex << type << std::endl;
							//std::cout << std::hex << (firstTileAddress + 4 * ((x * tileYCount) + y)) << std::endl;
							if (type == 1 || type == 3)
							{
								wallFound = true;
								break;
							}
						}
					}
					else
					{
						while (temp_pos.x > enemyPos.x || temp_pos.y > enemyPos.y)
						{
							//std::cout << "dusmanin sag altindayim" << std::endl;
							temp_pos = temp_pos + direction;
							//std::cout << temp_pos.x << std::endl;
							//std::cout << temp_pos.y << std::endl;
							float div = *(float*)(moduleBase + tilePixelSize);
							int x = PosToCell((double)((temp_pos.x) / div));
							int y = PosToCell((double)((temp_pos.y) / div));
							//std::cout << x << std::endl;
							//std::cout << y << std::endl;
							int type = *(int*)(firstTileAddress + 4 * ((x * tileYCount) + y));
							//std::cout << std::hex << type << std::endl;
							//std::cout << std::hex << (firstTileAddress + 4 * ((x * tileYCount) + y)) << std::endl;
							if (type == 1 || type == 3)
							{
								wallFound = true;
								break;
							}
						}
					}
				}

				if (len < maxLen && !wallFound)
				{
					maxLen = len;
					targetIndex = 1;
				}
				std::cout << "HORA DA VERDADE! : " << targetIndex << "\n";


		}


		// bodyguard.run(topWindow, playersPtrs, localPlayerPtr);

		// Player* enemy = aimbot.getClosestEnemy(playersPtrs, localPlayerPtr);
		// if (enemy)
		// {
		// 	aimbot.aimAt(localPlayerPtr, enemy);
		// }
			
		Sleep(30);
	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}