#ifndef SOUND_H
#define SOUND_H
/*


  File Name:		Sound.H

  Author:			David B. Harding
					15583 Kellogg Creek CT
					Lathrop CA 95330
					david@baycitygame.com

					Copyright (c) 2021-2023 Bay City Game
 
  Description:		

  Classification:	C++ Header File

  History / Status:	Work in progress.

  Known Bugs:		None

*/


class Sound : public  BaseAppObject
{

public:

	static DSCAPS& GetCaps(void);				// return dscaps object
	static LPDIRECTSOUND& GetDsound(void);		// return dsound object

	static BOOL Use_Sound(void);				// return value of use_sound

protected:
	Sound();
	virtual ~Sound();

	void Initialize(void);
	void Release(void);

	void ShowCaps( DSCAPS caps );		
	void Check (HRESULT value);			

private:

	HRESULT	hrRetVal;
	HRESULT QuerySoundCapability(void);

	static BOOL use_sound;				

	static DSCAPS			dscaps;
	static LPDIRECTSOUND	dsound;
	void Clear(DSCAPS& a_dscaps);		
};






#endif