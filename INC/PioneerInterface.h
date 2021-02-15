#ifndef __PIONEERINTERFACE_H__
#define __PIONEERINTERFACE_H__


#include "main.h"


class PioneerInterface
{
public:
	void VolUp_push(void);
	void VolDn_push(void);
	
	void TuneLt_push(void);
	void TuneLt_press(void);
	void TuneLt_release(void);
	
	void TuneRt_push(void);
	void TuneRt_press(void);
	void TuneRt_release(void);
	
	void BTAnswer(void);
	void BTReject(void);

	void FolderUp(void);
	void FolderDn(void);

	void Source(void);
	void Mute(void);
};


#endif
