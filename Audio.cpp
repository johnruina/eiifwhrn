
#include "Audio.h"

#define audio_throw_failed(hrcall) if (FAILED(hrcall)) throw;

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

Audio::Audio()
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    audio_throw_failed(::XAudio2Create(&xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR));

    audio_throw_failed(xaudio2->CreateMasteringVoice(&xaudio2masteringvoice,
        XAUDIO2_DEFAULT_CHANNELS,
        XAUDIO2_DEFAULT_SAMPLERATE,
        0,
        NULL,
        NULL,
        AudioCategory_GameEffects));

    WORD bitspersample = 16;
    DWORD samplespersec = 44100;

    WAVEFORMATEX waveformatex{};
    waveformatex.wFormatTag = WAVE_FORMAT_PCM;
    waveformatex.nChannels = 1; // 1 channel
    waveformatex.nSamplesPerSec = samplespersec;
    waveformatex.nBlockAlign = waveformatex.nChannels * bitspersample / 8;
    waveformatex.nAvgBytesPerSec = waveformatex.nSamplesPerSec * waveformatex.nBlockAlign;
    waveformatex.wBitsPerSample = bitspersample;
    waveformatex.cbSize = 0;
}

Audio::~Audio() {
    if (xaudio2)
    {
        xaudio2->Release();
    }

    // Uninitialize COM.
    CoUninitialize();

    return;
}

void Audio::PlayAudio(LPCWSTR filename)
{
    AudioData audiodata = LoadAudioData(filename);

    if (!audiodata.data) throw;

    XAUDIO2_BUFFER buffer = { 0 };

    buffer.PlayBegin = 0;
    buffer.PlayLength = 0;
    buffer.LoopBegin = 0;
    buffer.LoopCount = 0;
    buffer.LoopLength = 0;
    buffer.AudioBytes = audiodata.size;  //size of the audio buffer in bytes
    buffer.pAudioData = audiodata.data;  //buffer containing audio data
    buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
    IXAudio2SourceVoice* xaudio2sourcevoice;
    audio_throw_failed(xaudio2->CreateSourceVoice(&xaudio2sourcevoice, (WAVEFORMATEX*)&audiodata.format));
    audio_throw_failed(xaudio2sourcevoice->SubmitSourceBuffer(&buffer));
    audio_throw_failed(xaudio2sourcevoice->Start(0));
}

Audio::AudioData Audio::LoadAudioData(LPCWSTR filename)
{
    AudioData result = {};

    HANDLE file = CreateFileW(filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    DWORD dwChunkSize;
    DWORD dwChunkPosition;

    if (INVALID_HANDLE_VALUE == file) {
        throw;
    }
    if (INVALID_SET_FILE_POINTER == SetFilePointer(file, 0, NULL, FILE_BEGIN)) {

        throw;
    }
    GetChunk(file, fourccRIFF, dwChunkSize, dwChunkPosition);
    DWORD filetype;
    ReadChunkData(file, &filetype, sizeof(DWORD), dwChunkPosition);
    if (filetype != fourccWAVE) {
        throw "not a wav apparently";
    }

    GetChunk(file, fourccFMT, dwChunkSize, dwChunkPosition);
    ReadChunkData(file, &result.format, dwChunkSize, dwChunkPosition);

    GetChunk(file, fourccDATA, dwChunkSize, dwChunkPosition);
    BYTE* pDataBuffer = new BYTE[dwChunkSize];

    ReadChunkData(file, pDataBuffer, dwChunkSize, dwChunkPosition);

    CloseHandle(file);

    result.size = dwChunkSize;
    result.data = pDataBuffer;

    return result;
}

//shenanigans i dont want to touch

HRESULT Audio::GetChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition) {
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());
            break;

        default:
            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
                return HRESULT_FROM_WIN32(GetLastError());
        }

        dwOffset += sizeof(DWORD) * 2;

        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;

        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }

    return S_OK;
}

HRESULT Audio::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset) {
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());
    DWORD dwRead;
    if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
        hr = HRESULT_FROM_WIN32(GetLastError());
    return hr;
}

