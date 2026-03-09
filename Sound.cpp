#include "Sound.h"
#include "common.h"
#include "SoundSystem.h"

IXAudio2SourceVoice* Sound::GetSourceVoice()
{
    return m_sourceVoice;
}

X3DAUDIO_EMITTER Sound::GetEmitter()
{
    return m_emitter;
}

bool Sound::PlayTrack()
{
    HRESULT result;

    // Play the track.
    result = m_sourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
    if (FAILED(result))
    {
        std::cout << "TRACK FAILED TO PLAY";
        return false;
    }

    return true;
}

bool Sound::StopTrack()
{
    HRESULT result;


    // Play the track.
    result = m_sourceVoice->Stop(0);
    if (FAILED(result))
    {
        return false;
    }

    // Flush the buffers to remove them and reset the audio position to the beginning.
    result = m_sourceVoice->FlushSourceBuffers();
    if (FAILED(result))
    {
        return false;
    }

    // Resubmit the buffer to the source voice after the reset so it is prepared to play again.
    result = m_sourceVoice->SubmitSourceBuffer(&buffer);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void Sound::Update3DPosition(float x, float y, float z)
{
}

void Sound::InitializeEmitter()
{
    ZeroMemory(&m_emitter, sizeof(&m_emitter));

    m_emitter.ChannelCount = 1;
    m_emitter.CurveDistanceScaler = 1.0f;
    m_emitter.DopplerScaler = 1.0f;

    // Set an initial position for the sound.
    m_emitter.Position.x = 0.0f;
    m_emitter.Position.y = 0.0f;
    m_emitter.Position.z = 0.0f;

    return;
}

void Sound::LoadSoundData(SoundData* s)
{

    if (!s->data) throw;

    XAUDIO2_BUFFER buffer = { 0 };

    buffer.PlayBegin = 0;
    buffer.PlayLength = 0;
    buffer.LoopBegin = 0;
    buffer.LoopCount = 0;
    buffer.LoopLength = 0;
    buffer.AudioBytes = s->size;  //size of the audio buffer in bytes
    buffer.pAudioData = s->data;  //buffer containing audio data
    buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

    soundsystem->xaudio2->CreateSourceVoice(&m_sourceVoice, (WAVEFORMATEX*)&s->format);
    m_sourceVoice->SubmitSourceBuffer(&buffer);

}
