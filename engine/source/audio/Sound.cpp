#include "robot2DGame/experimental/audio/ALHelpers.hpp"
#include "robot2DGame/experimental/audio/Sound.hpp"

namespace robot2D {

    Sound::Sound():
            SoundSource(),
            m_soundBuffer{nullptr},
            m_loop{false} {}

    Sound::Sound(SoundBuffer* soundBuffer):
    SoundSource(),
    m_soundBuffer{soundBuffer},
    m_loop{false}
    {

    }

    void Sound::play() {
        alCall(alSourcePlay, ALSource);
    }

    void Sound::stop() {
        alCall(alSourceStop, ALSource);
    }

    void Sound::pause() {
        alCall(alSourcePause, ALSource);
    }

    void Sound::setupSource() {
        setPitch(1.F);
        setVolume(100.F);
        setLoop(false);

        alCall(alSourcei, ALSource, AL_BUFFER, m_soundBuffer -> getBufferID());
    }

    void Sound::setLoop(bool flag) {
        m_loop = flag;
        ALint looping = m_loop ? AL_TRUE : AL_FALSE;
        alCall(alSourcei, ALSource, AL_LOOPING, looping);
    }

    bool Sound::isLooping() const {
        return m_loop;
    }

    void Sound::setSoundBuffer(SoundBuffer* soundBuffer) {
        if(m_soundBuffer)
            stop();
        m_soundBuffer = soundBuffer;
        setupSource();
    }

    const SoundBuffer* Sound::getSoundBuffer() const {
        return m_soundBuffer;
    }
}