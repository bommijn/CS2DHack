#pragma once
#include <windows.h>
#include <cstdint>
#include <iostream>

struct nameObject {
	char pad_000C[12];
	wchar_t name[30];
};

class Player
{
public:
	int32_t placeholderForVtable; 
	int32_t N00000CCF; //0x0004
	int32_t id; //0x0008
	nameObject* ptr_to_name_struct;
	char pad_000C[176]; //0x000C
	int32_t used_in_aim_calc; //0x00BC //outdated/wrong
	int32_t used_in_aim_calc_also; //0x00C0 //outdated/wrong
	char pad_00C4[20]; //0x00C4
	int32_t cursordistancefromplayer; //0x00DC
	char pad_00E0[20]; //0x00E0
	int32_t when_not_moving_one_every_second_till_60; //0x00F4
	char pad_00F8[204]; //0x00F8
	int32_t team; //0x01C4
	char pad_01C8[12]; //0x01C8
	float xCoord; //0x01D4
	float yCoord; //0x01D8
	float viewAngleX; //0x01DC - might be wrong
	float viewAngleY; //0x01E0 - might be wrong
	int32_t something_with_X_pos_on_map; //0x01E4 - might be wrong
	int32_t something_with_y_pos_on_map; //0x01E8 - might be wrong
	char pad_01EC[0x8]; 
	int32_t ptr_to_health_encrypted;



	int get_health();
	bool is_dead();
	void test_offsets();
	/*virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();*/
}; //Size: 0x1068