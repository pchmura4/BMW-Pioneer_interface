#include "PioneerInterface.h"


void PioneerInterface::TuneLt_press(void)
{
	R12k_ON();
}

void PioneerInterface::TuneLt_release(void)
{
	R12k_OFF();
}

void PioneerInterface::TuneRt_press(void)
{
	R8k_ON();
}

void PioneerInterface::TuneRt_release(void)
{
	R8k_OFF();
}

void PioneerInterface::VolUp_push(void)
{
	R16k_ON();
	Delay(TIME);
	R16k_OFF();
}

void PioneerInterface::VolDn_push(void)
{
	R24k_ON();
	Delay(TIME);
	R24k_OFF();
}

void PioneerInterface::BTAnswer(void)
{
	RING_ON();
	R5k5_ON();
	R8k_ON();
	Delay(TIME);
	R5k5_OFF();
	R8k_OFF();
	RING_OFF();
}

void PioneerInterface::BTReject(void)
{
	RING_ON();
	R5k5_ON();
	Delay(TIME);
	R5k5_OFF();
	RING_OFF();
}

void PioneerInterface::FolderUp(void)
{
	RING_ON();
	R8k_ON();
	Delay(TIME);
	R8k_OFF();
	RING_OFF();
}

void PioneerInterface::FolderDn(void)
{
	RING_ON();
	R12k_ON();
	Delay(TIME);
	R12k_OFF();
	RING_OFF();
}

void PioneerInterface::Source(void)
{
	R1k2_ON();
	Delay(TIME);
	R1k2_OFF();
}

void PioneerInterface::Mute(void)
{
	R5k5_ON();
	R8k_ON();
	Delay(TIME);
	R5k5_OFF();
	R8k_OFF();
}
