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


uintptr_t localPlayerLinkedListOffset = 0x0486330;
Aimbot aimbot;
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


		Player* enemy = aimbot.getClosestEnemy(playersPtrs, localPlayerPtr);
		if (enemy)
		{
			aimbot.aimAt(localPlayerPtr, enemy);
		}
			
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























/*
DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"cs2d.exe");

	uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x00496e0c);
	std::cout << "LocalplayerPtr (static) = " << std::hex << localPlayerPtr << "\nLocalPlayerPointer derefrenced: " << *localPlayerPtr << std::endl <<std::endl;
	
	
	uintptr_t localPlayerAddr = moduleBase + 0x00496e0c;
	
	uintptr_t* vtPointer = (uintptr_t*)(localPlayerAddr);
	std::cout << "Adress of vtPointer" << std::hex << *vtPointer << std::endl;

	std::cout << "localPlayerPtr :" << std::hex << localPlayerPtr << "\n";

	// uintptr_t* positionYptr = (uintptr_t*)((*localPlayerPtr) + 0x1d8);
	// std::cout << "position address :" << std::hex << positionYptr << "\n";

	// float positionY = (float)(*positionYptr); 
	// std::cout << positionY << "\n";

	std::cout << "modulebase :" << std::hex << moduleBase << "\n";




	while (true)
	{
		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			int amountOfPlayer = 0;
			bool isOurPlayerFound = false;
			MODULEINFO moduleInfo;
			GetModuleInformation(GetCurrentProcess(), GetModuleHandle(L"cs2d.exe"), &moduleInfo, sizeof(moduleInfo));
			
			std::cout << "module -> SizeOfImage: " << moduleInfo.SizeOfImage;
			uintptr_t currentPosition = moduleBase;
			int fullModuleSize = moduleBase + moduleInfo.SizeOfImage - 4;
			std::cout << "Last searchable location " << fullModuleSize << std::endl;
			while (currentPosition < fullModuleSize)   
			{
				
					//std::cout << "currentPosition " << std::hex << currentPosition << " ";

					uintptr_t* possibleVTablePtr = (uintptr_t*)currentPosition;
					if (*vtPointer == *possibleVTablePtr) // checking the derefrenced value of the vTable
					{
						std::cout << "Found a player!!\n";
						std::cout << "possibleVTablePtrAddr = " << std::hex << *possibleVTablePtr <<
							 "VtablePtr Dynamic addr = " << *vtPointer <<std::endl;
						
						if (vtPointer != possibleVTablePtr)		// checking wether the STATIC adress of the vtable is not the same, to filter out our player.
						{
							std::cout << "The found player is not our player\n";
							amountOfPlayer++;
						}
						else
						{
							std::cout << "found player is us, Not adding it to the list.\n";
							isOurPlayerFound = true;
						}
					}
					
					
					//uintptr_t addressCouldBe = currentPosition + 0x1d8;
					//float possibleFloat = (float)(addressCouldBe);

					//std::cout << "possible float :" << std::dec <<  possibleFloat << "\n";

					/*if (possibleFloat > 700.0 && possibleFloat < 2400.0)
					{
						std::cout << "Found the player y position!" << "\n";
						std::cout << "possible address :" << addressCouldBe << "\n";
						std::cout << possibleFloat << "\n";
						amountOfPlayer++;
					}*/
					/*

				
				
				currentPosition++;
				
			}
			std::cout << "Amount of players found: " << amountOfPlayer << "\nIs our player found: " << isOurPlayerFound << std::endl;
			std::cout << "\n\n";

		}

		if(GetAsyncKeyState(VK_NUMPAD3) &  1)
		{
			std::cout << "localPlayerPtr :" << std::hex << localPlayerPtr << "\n";
			uintptr_t shootDirectionAddr = mem::FindDMAAddy(localPlayerAddr, { 0x1DC });

			float* shootDirection = (float*)shootDirectionAddr;
			std::cout << "adress of shoot dir: " << shootDirectionAddr << std::endl;
			std::cout << "Direction in which the bullet will fly = " << std::dec << *shootDirection << std::endl;
			std::cout << "local player vtable ptr " << std::hex << vtPointer << std::endl;
			
		}

		// THIS IS WORKING TO GET THE CURRENT PLAYER Y POSITION: 

		

	
		Sleep(20);
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
*/
