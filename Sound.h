#ifndef SOUND_CLASS
#define SOUND_CLASS

#include <xaudio2.h>
#include <x3daudio.h>

#include <glm/glm.hpp>

#include "Object.h"
#include "SoundSystem.h"

class Sound : public Object {
public:

    IXAudio2SourceVoice* GetSourceVoice();
    X3DAUDIO_EMITTER GetEmitter();

    bool PlayTrack();
    bool StopTrack();

    void Update3DPosition(float x, float y, float z);

public:
    void InitializeEmitter();
    
    void LoadSoundData(SoundData* s);
public:
    glm::vec3 pos;
    X3DAUDIO_EMITTER m_emitter;
    XAUDIO2_BUFFER buffer;
    IXAudio2SourceVoice* m_sourceVoice;
private:
};

#endif