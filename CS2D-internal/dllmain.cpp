#include "pch.h"
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <psapi.h>
#include "mem.h"


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