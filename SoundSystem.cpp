
#include "SoundSystem.h"
#include <iostream>

#define audio_throw_failed(hrcall) if (FAILED(hrcall)) throw;

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

SoundSystem::SoundSystem()
{
    xaudio2 = 0;
    masteringvoice = 0;
    matrixCoefficients = 0;

    HRESULT result;
    DWORD dwChannelMask;    


    // Initialize COM first.
    result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(result))
    {
        std::cout << "SOMETHING FUMBLED IN THE SOUNDSYSTEM CREATION1\m";
    }

    // Create an instance of the XAudio2 engine.
    result = XAudio2Create(&xaudio2, 0, XAUDIO2_USE_DEFAULT_PROCESSOR);
    if (FAILED(result))
    {
        std::cout << "SOMETHING FUMBLED IN THE SOUNDSYSTEM CREATION2\m";
    }

    // Create the mastering voice.
    result = xaudio2->CreateMasteringVoice(&masteringvoice);
    if (FAILED(result))
    {
        std::cout << "SOMETHING FUMBLED IN THE SOUNDSYSTEM CREATION3\m";
    }
    
    // Get the speaker setup for 3D audio settings.
    masteringvoice->GetChannelMask(&dwChannelMask);

    // Initialize X3DAudio.
    result = X3DAudioInitialize(dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, X3DInstance);
    if (FAILED(result))
    {
        std::cout << "SOMETHING FUMBLED IN THE SOUNDSYSTEM CREATION4\m";
    }
    
    ZeroMemory(&listener, sizeof(&listener));

    listener.Position.x = 0.0f;
    listener.Position.y = 0.0f;
    listener.Position.z = 0.0f;

    listener.OrientFront.x = 0.0f;
    listener.OrientFront.y = 0.0f;
    listener.OrientFront.z = 1.0f;

    listener.OrientTop.x = 0.0f;
    listener.OrientTop.y = 1.0f;
    listener.OrientTop.z = 0.0f;

    masteringvoice->GetVoiceDetails(&deviceDetails);
    // Create the matrix coefficients array for the DSP struct.
    matrixCoefficients = new float[deviceDetails.InputChannels];

    // Create an instance of the dsp settings structure.
    ZeroMemory(&DSPSettings, sizeof(&DSPSettings));

    DSPSettings.SrcChannelCount = 1;
    DSPSettings.DstChannelCount = deviceDetails.InputChannels;
    DSPSettings.pMatrixCoefficients = matrixCoefficients;
    
}

bool SoundSystem::Recalculate(X3DAUDIO_EMITTER emitter, IXAudio2SourceVoice* sourceVoice)
{
    
    HRESULT result;

    // Call X3DAudioCalculate to calculate new settings for the voices.

    X3DAudioCalculate(X3DInstance, &listener, &emitter, X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB, &DSPSettings);
    // Use SetOutputMatrix and SetFrequencyRatio to apply the volume and pitch values to the source voice.
    result = sourceVoice->SetOutputMatrix(masteringvoice, 1, deviceDetails.InputChannels, DSPSettings.pMatrixCoefficients);
    if (FAILED(result))
    {
        return false;
    }

    result = sourceVoice->SetFrequencyRatio(DSPSettings.DopplerFactor);
    if (FAILED(result))
    {
        return false;
    }
    
    return true;
}

SoundSystem::~SoundSystem() {
    if (matrixCoefficients)
    {
        delete[] matrixCoefficients;
        matrixCoefficients = 0;
    }
    if (masteringvoice) {
        masteringvoice->DestroyVoice();
    }
    if (xaudio2)
    {
        xaudio2->Release();
    }

    // Uninitialize COM.
    CoUninitialize();

    return;
}

SoundData* LoadFileToSoundData(LPCWSTR filename)
{
    SoundData* result = new SoundData();

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
    ReadChunkData(file, &result->format, dwChunkSize, dwChunkPosition);

    if (result->format.Format.nChannels != 1) {
        std::cout << "NON MONO WAV FILE\n";
    }

    GetChunk(file, fourccDATA, dwChunkSize, dwChunkPosition);
    BYTE* pDataBuffer = new BYTE[dwChunkSize];

    ReadChunkData(file, pDataBuffer, dwChunkSize, dwChunkPosition);

    CloseHandle(file);
    result->size = dwChunkSize;
    result->data = pDataBuffer; 
    return result;
}

//shenanigans i dont want to touch

HRESULT GetChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition) {
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

HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset) {
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());
    DWORD dwRead;
    if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
        hr = HRESULT_FROM_WIN32(GetLastError());
    return hr;
}

