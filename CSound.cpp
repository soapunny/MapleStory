#include "CSound.h"

FMOD_SYSTEM* CSound::g_sound_system;

CSound::CSound(const char* path, bool loop) {
    if (loop) {
        FMOD_System_CreateSound(g_sound_system, path, FMOD_LOOP_NORMAL, 0, &m_sound);
    }
    else {
        FMOD_System_CreateSound(g_sound_system, path, FMOD_DEFAULT, 0, &m_sound);
    }

    m_channel = nullptr;
    m_volume = SOUND_DEFAULT;
}

CSound::~CSound() {
    FMOD_Sound_Release(m_sound);
}


int CSound::Init() {
    FMOD_System_Create(&g_sound_system);
    FMOD_System_Init(g_sound_system, 32, FMOD_INIT_NORMAL, nullptr);

    return 0;
}

int CSound::Release() {
    FMOD_System_Close(g_sound_system);
    FMOD_System_Release(g_sound_system);

    return 0;
}


int CSound::play() {
    FMOD_System_PlaySound(g_sound_system, m_sound, nullptr, false, &m_channel);

    return 0;
}

int CSound::pause() {
    FMOD_Channel_SetPaused(m_channel, true);

    return 0;
}

int CSound::resume() {
    FMOD_Channel_SetPaused(m_channel, false);

    return 0;
}

int CSound::stop() {
    FMOD_Channel_Stop(m_channel);
    return 0;
}

int CSound::volumeUp() {
    if (m_volume < SOUND_MAX) {
        m_volume += SOUND_WEIGHT;
    }

    FMOD_Channel_SetVolume(m_channel, m_volume);

    return 0;
}

int CSound::volumeDown() {
    if (m_volume > SOUND_MIN) {
        m_volume -= SOUND_WEIGHT;
    }

    FMOD_Channel_SetVolume(m_channel, m_volume);

    return 0;
}


int CSound::Update() {
    FMOD_Channel_IsPlaying(m_channel, &m_bool);

    if (m_bool) {
        FMOD_System_Update(g_sound_system);
    }
    else
    {
        //계속 플레이
        stop();
        play();
    }

    return 0;
}