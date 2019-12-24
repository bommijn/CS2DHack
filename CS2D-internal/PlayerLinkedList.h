#pragma once
#include <Windows.h>
#include <vector>


struct PlayerLinkedList {
	char pad_000[8];
	uintptr_t playerBasePointer;
	PlayerLinkedList* nextOnList;
	PlayerLinkedList* previousOnList;
};