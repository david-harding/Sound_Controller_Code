/*


  File Name:		Sound.Cpp

  Author:			David B. Harding
					15583 Kellogg Creek CT
					Lathrop CA 95330
					david@baycitygame.com

					Copyright (c) 2021-2023 Bay City Game
 
  Description:		

  Classification:	C++ Compilation Unit

  History / Status:	Work in progress.

  Known Bugs:		None

*/
#include "StdAfx.h"
#include "sound.h"
#include "message.h"
#include "video.h"

LPDIRECTSOUND Sound::dsound;
DSCAPS Sound::dscaps;
BOOL Sound::use_sound;


Sound::Sound()
{

	TRACE("Sound::Sound invoked\n");
	use_sound = TRUE;					// set to FALSE if no sound driver is loaded
	Clear(dscaps);						// clear the dscaps object
	
}

Sound::~Sound()
{

	TRACE("Sound::~Sound invoked\n");

}

void Sound::Initialize(void)
{

	TRACE("Sound::Initialize invoked\n");

	HWND hwnd = Video::GetHwnd();

	hrRetVal = DirectSoundCreate(NULL, &dsound, NULL);

	if (DS_OK == hrRetVal)
	{
		hrRetVal = QuerySoundCapability();	

		if (DS_OK == hrRetVal)
			hrRetVal = dsound->SetCooperativeLevel(hwnd, DSSCL_NORMAL);
		else
		{
			Check(hrRetVal);
			TRACE("Sound::Initialize setting use_sound to FALSE\n");
			use_sound = FALSE;	// disable all subsequent calls to direct sound
		}
		
	}
	else
	{
		Check(hrRetVal);
	
		InfoMessageBox((char*)"Can not initialize sound device.");

		TRACE("Sound::Initialize setting use_sound to FALSE\n");
		use_sound = FALSE;		// disable all subsequent calls to direct sound
	}

}


void Sound::Release(void)
{

	if (use_sound)
	{
		TRACE("Sound::Release invoked\n");
		dsound->Release();
	}

}

HRESULT Sound::QuerySoundCapability(void)
{

	if (use_sound)
	{
		TRACE("Sound::QuerySoundCapability invoked\n");

		hrRetVal = dsound->GetCaps(&dscaps);
		if (DS_OK == hrRetVal)
			ShowCaps(dscaps);
		else
			Check(hrRetVal);

	}

	return(hrRetVal);


	


}

DSCAPS& Sound::GetCaps(void)
{

	return(dscaps);

}

void Sound::Clear(DSCAPS& a_dscaps)
{
	memset(&a_dscaps, 0, sizeof(a_dscaps));
	a_dscaps.dwSize = sizeof(a_dscaps);
}


LPDIRECTSOUND& Sound::GetDsound(void)
{

	return(dsound);

}


void Sound::ShowCaps( DSCAPS caps )
{

	if (use_sound)
	{
		TRACE("Direct Sound Capablilities: ");

		if ((DSCAPS_CERTIFIED & caps.dwFlags) == DSCAPS_CERTIFIED)
			TRACE("DSCAPS_CERTIFIED ");

		if ((DSCAPS_CONTINUOUSRATE & caps.dwFlags) == DSCAPS_CONTINUOUSRATE)
			TRACE("DSCAPS_CONTINUOUSRATE ");

		if ((DSCAPS_EMULDRIVER & caps.dwFlags) == DSCAPS_EMULDRIVER)
			TRACE("DSCAPS_EMULDRIVER ");

		if ((DSCAPS_PRIMARY16BIT & caps.dwFlags) == DSCAPS_PRIMARY16BIT)
			TRACE("DSCAPS_PRIMARY16BIT ");

		if ((DSCAPS_PRIMARY8BIT & caps.dwFlags) == DSCAPS_PRIMARY8BIT)
			TRACE("DSCAPS_PRIMARY8BIT ");

		if ((DSCAPS_PRIMARYMONO & caps.dwFlags) == DSCAPS_PRIMARYMONO)
			TRACE("DSCAPS_PRIMARYMONO ");

		if ((DSCAPS_PRIMARYSTEREO & caps.dwFlags) == DSCAPS_PRIMARYSTEREO)
			TRACE("DSCAPS_PRIMARYSTEREO ");

		if ((DSCAPS_SECONDARY16BIT & caps.dwFlags) == DSCAPS_SECONDARY16BIT)
			TRACE("DSCAPS_SECONDARY16BIT ");

		if ((DSCAPS_SECONDARY8BIT & caps.dwFlags) == DSCAPS_SECONDARY8BIT)
			TRACE("DSCAPS_SECONDARY8BIT ");

		if ((DSCAPS_SECONDARYMONO & caps.dwFlags) == DSCAPS_SECONDARYMONO)
			TRACE("DSCAPS_SECONDARYMONO ");

		if ((DSCAPS_SECONDARYSTEREO & caps.dwFlags) == DSCAPS_SECONDARYSTEREO)
			TRACE("DSCAPS_SECONDARYSTEREO ");

		TRACE("\n");

	}

}


void Sound::Check (HRESULT value)
{

	switch (value)
	{
		case DSERR_ALLOCATED :
			TRACE("DSERR_ALLOCATED\n");
			break;
		case DSERR_ALREADYINITIALIZED :
			TRACE("DSERR_ALREADYINITIALIZED\n");
			break;
		case DSERR_BADFORMAT :
			TRACE("DSERR_BADFORMAT\n");
			break;
		case DSERR_GENERIC :
			TRACE("DSERR_GENERIC\n");
			break;
		case DSERR_INVALIDCALL :
			TRACE("DSERR_INVALIDCALL\n");
			break;
		case DSERR_INVALIDPARAM :
			TRACE("DSERR_INVALIDPARAM\n");
			break;
		case DSERR_NOAGGREGATION :
			TRACE("DSERR_NOAGGREGATION\n");
			break;
		case DSERR_NODRIVER :
			TRACE("DSERR_NODRIVER\n");
			break;
		case DSERR_OUTOFMEMORY :
			TRACE("DSERR_OUTOFMEMORY\n");
			break;
		case DSERR_PRIOLEVELNEEDED :
			TRACE("DSERR_PRIOLEVELNEEDED\n");
			break;
		case DSERR_UNINITIALIZED :
			TRACE("DSERR_UNINITIALIZED\n");
			break;
		case DSERR_UNSUPPORTED :
			TRACE("DSERR_UNSUPPORTED\n");
			break;
		default:
			TRACE("UNKNOWN RETURN VALUE\n");
			break;
	}
}

BOOL Sound::Use_Sound(void)
{

	return(use_sound);

}