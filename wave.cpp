/*


  File Name:		Wave.Cpp

  Author:		David B. Harding
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
#include "wave.h"
#include "message.h"
#include "sound.h"

Wave::Wave(const char* file_name)
{
	TRACE("Wave::Wave invoked\n");

	Clear(desc);						
	Clear(wave);						
	dsbuf = NULL;

	Read(file_name);

}

Wave::~Wave()
{
	TRACE("Wave::~Wave invoked\n");
}


void Wave::Read (const char* file_name)
{
	if (Sound::Use_Sound())
	{

		try
		{
			TRACE("Wave::Read invoked\n");

			HMMIO       file;			// file handle for open file 
			MMCKINFO    parent_chunk;	// parent chunk information 
			MMCKINFO    sub_chunk;		// subchunk information structure 
			DWORD       dwFmtSize;		// size of "FMT" chunk 
			DWORD       dwDataSize;		// size of "DATA" chunk 
			BYTE		*lpData;		// address of "DATA" chunk 
			DSBUFFERDESC SoundBuffer;	// Characteristics of new sound buffer used by IDirectSound8::CreateSoundBuffer

			PCMWAVEFORMAT  pcmFormat;	// address of "FMT" chunk 
   

			DWORD		dwBSize, dwWrapBSize;

			BYTE		*lpWrapPtr = NULL;
			BYTE        *lpWritePtr = NULL;

			LPDIRECTSOUND dsound = Sound::GetDsound();

			
			Clear(SoundBuffer);
 
			if(!(file = mmioOpen((LPSTR)file_name, NULL, MMIO_READ | MMIO_ALLOCBUF))) 
			{ 
				TRACE ("Wave::Read %s not found\n", file_name);
				char message[60];

				strcpy ( message, "Wave file not found: ");
				strcat ( message, file_name);
				
				InfoMessageBox( message );
			
				throw (WAVE_EXCEPTION); 
			} 
 
			parent_chunk.fccType = mmioFOURCC('W', 'A', 'V', 'E'); 
			if (mmioDescend(file, (LPMMCKINFO) &parent_chunk, NULL, MMIO_FINDRIFF)) 
			{ 
				TRACE("Wave::Read %s is not a waveform-audio file\n", file_name); 
				mmioClose(file, 0); 
				throw (WAVE_EXCEPTION); 
			} 
			
			sub_chunk.ckid = mmioFOURCC('f', 'm', 't', ' '); 
			if (mmioDescend(file, &sub_chunk, &parent_chunk, MMIO_FINDCHUNK)) 
			{ 
				TRACE("Wave::Read %s waveform-audio file has no FMT chunk\n", file_name); 
				mmioClose(file, 0); 
				throw (WAVE_EXCEPTION);  
			} 
 
			dwFmtSize = sub_chunk.cksize; 
			if (mmioRead(file, (HPSTR) &pcmFormat, (long) sizeof(pcmFormat) ) !=  (long) sizeof(pcmFormat))
			{ 
				TRACE("Wave::Read %s failed to read format chunk\n", file_name); 
				mmioClose(file, 0); 
				throw (WAVE_EXCEPTION); 
			} 
 
			// Ascend out of the "FMT" subchunk. 
			mmioAscend(file, &sub_chunk, 0); 
 
			// Find the data subchunk. The current file position should be at 
			// the beginning of the data chunk; however, you should not make 
			// this assumption. Use mmioDescend to locate the data chunk. 
			sub_chunk.ckid = mmioFOURCC('d', 'a', 't', 'a'); 
			if (mmioDescend(file, &sub_chunk, &parent_chunk, 
				MMIO_FINDCHUNK)) 
			{ 
				TRACE("Wave::Read %s waveform-audio file has no data chunk\n", file_name ); 
				//. 
				//. 
				//. 
				mmioClose(file, 0); 
				throw (WAVE_EXCEPTION); 
			} 
 
			// Get the size of the data subchunk. 
			dwDataSize = sub_chunk.cksize; 
			if (dwDataSize == 0L){ 
				TRACE("Wave::Read %s the data chunk contains no data\n", file_name); 
				mmioClose(file, 0); 
				throw (WAVE_EXCEPTION); 
			} 
 
			
			lpData = new BYTE[dwDataSize];
 
			// Read the waveform-audio data subchunk. 
			if(mmioRead(file,(HPSTR)lpData, dwDataSize) != dwDataSize){ 
				TRACE("Wave::Read %s failed to read data chunk\n", file_name); 
				mmioClose(file, 0); 
				throw (WAVE_EXCEPTION); 
			} 

			// Close the file. 
			mmioClose(file, 0); 
 
    
			
			memcpy( &wave.wf, &pcmFormat, sizeof(pcmFormat) );


			// setup desc structure
			// DEPRECATED desc.dwFlags = DSBCAPS_CTRLDEFAULT;
			desc.dwFlags = SoundBuffer.dwFlags;
			// three second buffer
			desc.dwBufferBytes = dwDataSize;
			desc.lpwfxFormat = (LPWAVEFORMATEX)&wave;
			//
			hrRetVal = dsound->CreateSoundBuffer(&desc, &dsbuf, NULL);
			if (DS_OK != hrRetVal)
			{
				dsbuf = NULL;	// prevent this from ever being played
				Check(hrRetVal);
			}
			else
			{
				hrRetVal = dsbuf->Lock(	0,  dwDataSize,   (LPVOID*) &lpWritePtr, &dwBSize,
									    (LPVOID*) &lpWrapPtr,	&dwWrapBSize, 0L);
				// HRESULT Lock( DWORD dwWriteCursor, DWORD dwWriteBytes, LPVOID lplpvAudioPtr1, LPDWORD lpdwAudioBytes1, LPVOID lplpvAudioPtr2, LPDWORD lpdwAudioBytes2, DWORD dwFlags );
				if (DS_OK != hrRetVal)
				{
					dsbuf = NULL;	// prevent this from ever being played
					Check(hrRetVal);
				}
				else
				{
					memcpy(lpWritePtr, lpData, dwDataSize);

					if (dwWrapBSize)
						memcpy(lpWrapPtr, lpData+dwBSize, dwWrapBSize);
				}

				hrRetVal = dsbuf->Unlock( lpWritePtr, dwBSize, lpWrapPtr, dwWrapBSize);

				if (DS_OK != hrRetVal)
					Check(hrRetVal);
			}
			
			delete (lpData);

		} // end try

		catch ( int exception )
		{
			TRACE("%Xh exception caught by Wave::Read\n", exception);

		} // end catch

	} // end if

}


void Wave::Play(void)
{
	TRACE("Wave::Play Invoked\n");

	if (Sound::Use_Sound())
	{
		//TRACE("Wave::Play invoked\n");

		if (dsbuf != NULL)
		{
			hrRetVal = dsbuf->Play(0,0,0);
			if (DS_OK != hrRetVal)
				Check(hrRetVal);
		}

	}
}

void Wave::Clear(PCMWAVEFORMAT& a_wave)
{
	memset(&a_wave, 0, sizeof(a_wave));
}

void Wave::Clear(DSBUFFERDESC& a_desc)
{
	memset(&a_desc, 0, sizeof(a_desc));
	a_desc.dwSize = sizeof(a_desc);

}

void Wave::Check (HRESULT value)
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