#include "Utils.h"

uintptr_t Utils::SendChatMessageToAll(uintptr_t moduleBase, uintptr_t textPtr ) {
	// std::cout << "TEXT AQUI text aqui" << "\n";
	// std::cout << (std::wcslen((wchar_t*)textPtr)) << "\n";

	unsigned char toBeCopied[63] = {0xA0, 0x4C, 0x7C, 0x00, 0x01, 0x00, 0x00, 0x00, (unsigned char)(std::wcslen((wchar_t*)textPtr)), 0x00, 0x00, 0x00};
	
	unsigned char *chatStringPtr = new unsigned char[63];

	memcpy(chatStringPtr, (void*)&toBeCopied[0],  63);

	// std::copy(chatStringPtr, chatStringPtr + 63, &toBeCopied[0]);

	memcpy(chatStringPtr + 0xC , (void*)textPtr, (std::wcslen((wchar_t*)textPtr)*2));

	// std::cout << "DASmdlsdKAld DSKLADKlDSAKl DSakl JDASKL DJASKL chatStringPtr" << "\n";
	// std::cout << std::hex << static_cast<void*>(chatStringPtr) << "\n";

	unsigned int arrayOfBytesPtr = (unsigned int)chatStringPtr;
	unsigned int chatFuncAddress = (unsigned int)moduleBase + 0x2CA805;

	__asm{
	  mov eax, 0x1;
	  mov edx, 0x1;
	  mov edi, 0x22;
	  mov esi, 0x4B;
	  mov ebx, 0x4A;
	  push 0x0;
	  push 0x1;
	  push [arrayOfBytesPtr];
	  push 0x1;
	  push 0x1;
	  call chatFuncAddress;
	  add  ESP,0x14;  
	};	

	return 0;
}
