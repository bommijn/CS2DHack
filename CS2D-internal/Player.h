#pragma once
#include <windows.h>
#include <cstdint>
class Player
{
public:
	int32_t whatever; 
	int32_t N00000CCF; //0x0004
	int32_t N0000099F; //0x0008
	char pad_000C[176]; //0x000C
	int32_t used_in_aim_calc; //0x00BC
	int32_t used_in_aim_calc_also; //0x00C0
	char pad_00C4[24]; //0x00C4
	int32_t cursordistancefromplayer; //0x00DC
	char pad_00E0[20]; //0x00E0
	int32_t when_not_moving_one_every_second_till_60; //0x00F4
	char pad_00F8[204]; //0x00F8
	int32_t team; //0x01C4
	char pad_01C8[12]; //0x01C8
	float xCoord; //0x01D4
	float yCoord; //0x01D8
	float viewAngleX; //0x01DC
	float viewAngleY; //0x01E0
	int32_t something_with_X_pos_on_map; //0x01E4
	int32_t something_with_y_pos_on_map; //0x01E8
	char pad_01EC[96]; //0x01EC
	class currentGun *N00000D66; //0x024C
	char pad_0250[120]; //0x0250
	int32_t Xcoord_mouse_on_screen; //0x02C8
	int32_t Ycoord_mouse_on_screen; //0x02CC
	char pad_02D0[104]; //0x02D0
	float N00000DA3; //0x0338
	char pad_033C[3372]; //0x033C
	
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