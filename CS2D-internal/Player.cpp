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
void Player::test_offsets() {
	std::cout << "My instance is at:" << std::hex << this << std::endl;
	std::cout << "My placeholderForVtable is at:" << std::hex << (uintptr_t)&placeholderForVtable - (uintptr_t)this << std::endl;
	std::cout << "My N00000CCF is at:" << std::hex << (uintptr_t)&N00000CCF - (uintptr_t)this << std::endl;
	std::cout << "My id is at:" << std::hex << (uintptr_t)&id - (uintptr_t)this << std::endl;
	std::cout << "My ptr_to_name_struct is at:" << std::hex << (uintptr_t)&ptr_to_name_struct - (uintptr_t)this << std::endl;
	std::cout << "My pad_000C is at:" << std::hex << (uintptr_t)&pad_000C - (uintptr_t)this << std::endl;
	std::cout << "My used_in_aim_calc is at:" << std::hex << (uintptr_t)&used_in_aim_calc - (uintptr_t)this << std::endl;
	std::cout << "My used_in_aim_calc_also is at:" << std::hex << (uintptr_t)&used_in_aim_calc_also - (uintptr_t)this << std::endl;
	std::cout << "My pad_00C4 is at:" << std::hex << (uintptr_t)&pad_00C4 - (uintptr_t)this << std::endl;
	std::cout << "My cursordistancefromplayer is at:" << std::hex << (uintptr_t)&cursordistancefromplayer - (uintptr_t)this << std::endl;
	std::cout << "My pad_00E0 is at:" << std::hex << (uintptr_t)&pad_00E0 - (uintptr_t)this << std::endl;
	std::cout << "My when_not_moving_one_every_second_till_60 is at:" << std::hex << (uintptr_t)&when_not_moving_one_every_second_till_60 - (uintptr_t)this << std::endl;
	std::cout << "My pad_00F8 is at:" << std::hex << (uintptr_t)&pad_00F8 - (uintptr_t)this << std::endl;
	std::cout << "My team is at:" << std::hex << (uintptr_t)&team - (uintptr_t)this << std::endl;
	std::cout << "My pad_01C8 is at:" << std::hex << (uintptr_t)&pad_01C8 - (uintptr_t)this << std::endl;
	std::cout << "My xCoord is at:" << std::hex << (uintptr_t)&xCoord - (uintptr_t)this << std::endl;
	std::cout << "My yCoord is at:" << std::hex << (uintptr_t)&yCoord - (uintptr_t)this << std::endl;
	std::cout << "My viewAngleOne is at:" << std::hex << (uintptr_t)&viewAngleOne - (uintptr_t)this << std::endl;
	std::cout << "My viewAngleTwo is at:" << std::hex << (uintptr_t)&viewAngleTwo - (uintptr_t)this << std::endl;
	std::cout << "My something_with_X_pos_on_map is at:" << std::hex << (uintptr_t)&something_with_X_pos_on_map - (uintptr_t)this << std::endl;
	std::cout << "My something_with_y_pos_on_map is at:" << std::hex << (uintptr_t)&something_with_y_pos_on_map - (uintptr_t)this << std::endl;
	std::cout << "My pad_01EC is at:" << std::hex << (uintptr_t)&pad_01EC - (uintptr_t)this << std::endl;
	std::cout << "My ptr_to_health_encrypted is at:" << std::hex << (uintptr_t)&ptr_to_health_encrypted - (uintptr_t)this << std::endl;
}