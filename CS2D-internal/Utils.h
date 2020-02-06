#pragma once
#include <iomanip>
#include <cwchar>
#include <iostream>

class Utils
{
public:
	Utils();
	~Utils();
	
	static uintptr_t SendChatMessageToAll(uintptr_t moduleBase, uintptr_t textPtr );
};
