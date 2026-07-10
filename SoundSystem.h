#ifndef SOUND_SYSTEM_CLASS
#define SOUND_SYSTEM_CLASS

#define NOMINMAX

#include <Windows.h>
#include <wrl.h>
#include <xaudio2.h>
#include <x3daudio.h>
#include <vector>

#pragma comment(lib, "Xaudio2.lib")

namespace wrl = Microsoft::WRL;

struct SoundData {
	BYTE* data;
	UINT32 size;
	WAVEFORMATEXTENSIBLE format;
};

HRESULT GetChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
SoundData* LoadFileToSoundData(LPCWSTR name);

class SoundSystem {
public:
	//funcs
	SoundSystem();
	~SoundSystem();
	void PlayAudio(LPCWSTR filename);
	bool Recalculate(X3DAUDIO_EMITTER emitter, IXAudio2SourceVoice* sourceVoice);	
private:
	//pfuncs
public:
	IXAudio2* xaudio2;
	IXAudio2MasteringVoice* masteringvoice;

	//pvariables
	X3DAUDIO_HANDLE X3DInstance;
	X3DAUDIO_LISTENER listener;
	float* matrixCoefficients;
	XAUDIO2_VOICE_DETAILS deviceDetails;
	X3DAUDIO_DSP_SETTINGS DSPSettings;
};

#endif