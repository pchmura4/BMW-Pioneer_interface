#include "BMWbus.h"


bool BMWbus::isAvailable(void)
{
	bool result = available;
	available = false;
	return result;
}


BusMsg BMWbus::interpret(void)
{
	if(available)
	{
		if(sourceId == 0x50)
		{
			if((destId == 0x68) && (length == 4))
			{
				if(data[0] == 0x3B)
				{
					switch (data[1])
					{
						case 0x01:
							return Next_press;
						case 0x11:
							return Next_1s;
						case 0x21:
							return Next_release;
						case 0x08:
							return Prev_press;
						case 0x18:
							return Prev_1s;
						case 0x28:
							return Prev_release;
					}
				}
				else if(data[0] == 0x32)
				{
					switch (data[1])
					{
						case 0x11:
							return VolUp_press;
						case 0x31:
							return VolUp_release;
						case 0x10:
							return VolDn_press;
						case 0x30:
							return VolDn_release;
					}
				}
			}
			else if(destId == 0xC8)
			{
				if((data[0] == 0x01) && (length == 3))
				{
					return RT_pushed;
				}
				else if((data[0] == 0x3B) && (length == 4))
				{
					switch (data[1])
					{
						case 0x80:
							return Dial_press;
						case 0x90:
							return Dial_1s;
						case 0xA0:
							return Dial_release;
					}
				}
			}
		}
		else if(sourceId == 0x80)
		{
			if(destId == 0xBF)
			{
				if(data[0] == 0x11)
				{
					switch(data[1])
					{
						case 0x01:
							return Pos1_Acc;
						case 0x02:
							return Pos2_On;
						case 0x04:
							return Pos3_Start;
					}
				}
			}
		}
	}
	return UNKNOWN;
}


void BMWbus::parse(uint8_t byte)
{
	static uint8_t state=0, msg_len, dat_ptr;

	for(;;)
	{
		switch(state)
		{
			case 0:
			{
				if((byte == 0x50) || (byte == 0x80))
				{
					state = 1;
					msg_len=0; dat_ptr=0;
					sourceId = byte;
				}
			} return;

			case 1:
			{	state = 0;
				if((byte >= 0x03) && (byte <= 0x22))
				{
					msg_len = length = (byte);
					state = 2;
				}
				else continue;
			} return;

			case 2:
			{	state = 0;
				if(--msg_len)
				{
					destId = byte;
					state = 3;
				}
				else continue;
			} return;

			case 3:
			{	state = 4;
				if(--msg_len)
				{
					data[dat_ptr++] = byte;
					state = 3;
				}
				else continue;
			} return;

			case 4:
			{	state = 0;
				checksum = byte;
				if(calculate_chk() == 0)
					available = true;
			} return;
		}
	}
}


uint8_t BMWbus::calculate_chk(void)
{
	uint8_t	chk = 0;

	chk ^= sourceId;
	chk ^= length;
	chk ^= destId;

	for(int i=0; i<(length-2); i++)
		chk ^= data[i];

	chk ^= checksum;

	return chk;
}
