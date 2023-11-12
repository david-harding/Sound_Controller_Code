#ifndef WAVE_H
#define WAVE_H
/*


  File Name:		Wave.H

  Author:			David B. Harding
					15583 Kellogg Creek CT
					Lathrop CA 95330
					david@baycitygame.com

					Copyright (c) 2021-2023 Bay City Game
 
  Description:		This class provides a high level interface to the Direct Sound
					API.

  Classification:	C++ Header File

  History / Status:	Work in progress.

  Known Bugs:		None

*/

#include "BaseAppObject.h"
class Wave : public BaseAppObject
{
public:
	Wave(const char* file_name);
	virtual ~Wave();

	void Play (void);
	
	
protected:
	
	
	
private:

	void Read (const char* file_name);

	PCMWAVEFORMAT	wave;
	DSBUFFERDESC	desc;
	HRESULT			hrRetVal;
			
	LPDIRECTSOUNDBUFFER dsbuf;

	void Clear(PCMWAVEFORMAT& a_wave);
	void Clear(DSBUFFERDESC& a_desc);
	void Check(HRESULT value);

};


#endif