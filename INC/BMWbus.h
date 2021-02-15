#ifndef __BMWBUS_H__
#define __BMWBUS_H__


#include "main.h"


enum BusMsg
{
	VolUp_press,
	VolUp_release,
	VolDn_press,
	VolDn_release,
	Next_press,
	Next_1s,
	Next_release,
	Prev_press,
	Prev_1s,
	Prev_release,
	RT_pushed,
	Dial_press,
	Dial_1s,
	Dial_release,
	Pos1_Acc,
	Pos2_On,
	Pos3_Start,
	UNKNOWN
};


class BMWbus
{
public:
	void parse(uint8_t byte);
	bool isAvailable(void);
	BusMsg interpret(void);

private:
	uint8_t calculate_chk(void);
	uint8_t sourceId;
	uint8_t length;
	uint8_t destId;
	uint8_t data[32];
	uint8_t checksum;
	bool available;
};


#endif
