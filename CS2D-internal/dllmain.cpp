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
#include "Bodyguard.h"


uintptr_t localPlayerLinkedListOffset = 0x0486330;
Aimbot aimbot;
Bodyguard bodyguard;
std::vector<uintptr_t> realPlayerBases;
std::vector<uintptr_t> localPlayerLinkedListPointers = { 0x8, 0xc, 0x0 };

std::vector<uintptr_t> filterRealPlayerBases(std::vector<uintptr_t> possiblePlayerBases, uintptr_t knownLocalPlayerVtable) {

	std::cout << "AEWS knownLocalPlayerVtable :" << std::hex << knownLocalPlayerVtable << "\n";


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

void SendKeyInput(HWND &wHANDLE, WPARAM key)
{
	PostMessage(wHANDLE, WM_KEYDOWN, key, 0);
	Sleep(50);
	PostMessage(wHANDLE, WM_KEYUP, key, 0);
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
	

	while (true)
	{

		HWND topWindow = FindTopWindow(GetCurrentProcessId());
		SendKeyInput(topWindow, 0x57);

		// std::cout << "localPlayerPtr :" << std::hex << *localPlayerPtr << "\n";

		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{

		}

		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			std::cout << "localPlayerPtr base value:" << std::hex << *(uintptr_t*)localPlayerPtr << "\n";
			// float shootDirection = *(float*)((*localPlayerPtr)+0x1DC );

			std::cout << "Direction in which the bullet will fly = " << std::fixed << std::setprecision(3) << localPlayerPtr->viewAngleX << std::endl;
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
			std::cout << (uintptr_t)player << std::endl;
			std::cout << "Player Base :" << (uintptr_t)(player) << "\n";
			std::cout << "Player Position X :" << player->xCoord << "\n";
			std::cout << "Player Position Y :" << player->yCoord << "\n";
			std::cout << "---------------------- \n";
			playersPtrs.push_back(player);
		};

		bodyguard.run(playersPtrs, localPlayerPtr);

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