#ifndef SOUND_CLASS
#define SOUND_CLASS

#include <Windows.h>
#include <wrl.h>
#include <xaudio2.h>
#include <x3daudio.h>
#include <vector>

namespace wrl = Microsoft::WRL;



class SoundSystem {
public:
	//funcs
	SoundSystem();
	~SoundSystem();
	void PlayAudio(LPCWSTR filename);

	struct AudioData {
		BYTE* data;
		UINT32 size;
		WAVEFORMATEXTENSIBLE format;
	};

	struct AudioObject {
		IXAudio2SourceVoice* sourcevoice;
		unsigned short int id;
	};

	AudioData LoadAudioData(LPCWSTR name);
private:
	//pfuncs
	HRESULT GetChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
private:
	X3DAUDIO_LISTENER Listener = {};

	X3DAUDIO_DSP_SETTINGS DSPSettings = {};

	//pvariables
	X3DAUDIO_HANDLE X3DInstance;
	wrl::ComPtr<IXAudio2> xaudio2{};
	IXAudio2MasteringVoice* xaudio2masteringvoice;
	AudioObject sources[16] = {};
};

#endif