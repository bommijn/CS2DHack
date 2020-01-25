#include "Player.h"

int Player::get_health() {
	// std::cout << "PLAYER_ID :" << std::dec << id << "\n";
	// std::cout << "localondetoolhando :" << std::dec << &ptr_to_health_encrypted << "\n";
	// std::cout << "AEwdjasKDjsakdjsaKdlas :" << std::hex << ptr_to_health_encrypted << "\n";
	int firstEncryptedValue = (*(int*)(ptr_to_health_encrypted + 0x8));
	// std::cout << "AEwdjasKDjsakdjsaKdlas :" << std::dec << firstEncryptedValue << "\n";
	int secondEncryptedValue = (*(int*)(ptr_to_health_encrypted + 0xC));
	// std::cout << "AEwdjasKDjsakdjsaKdlas :" << std::dec << secondEncryptedValue << "\n";
	return firstEncryptedValue + secondEncryptedValue;
}

bool Player::is_dead() {
	int health = get_health();
	return health <=0;
}