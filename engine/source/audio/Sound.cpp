#include "robot2DGame/experimental/audio/ALHelpers.hpp"
#include "robot2DGame/experimental/audio/Sound.hpp"

namespace robot2D {

    Sound::Sound():
            SoundSource(),
            m_soundBuffer{nullptr},
            m_loop{false} {}

    Sound::Sound(const SoundBuffer* soundBuffer):
    SoundSource(),
    m_soundBuffer{nullptr},
    m_loop{false}
    {
        setSoundBuffer(*soundBuffer);
    }

    Sound::Sound(const Sound& copy): SoundSource(copy) {
        if(copy.m_soundBuffer)
            setSoundBuffer(*copy.m_soundBuffer);
        setLoop(copy.m_loop);
    }

    Sound& Sound::operator=(const Sound& copy) {
        if (this == &copy)
            return *this;

        SoundSource::operator=(copy);
        if(m_soundBuffer) {
            stop();
            m_soundBuffer = nullptr;
        }

        if(copy.m_soundBuffer)
            setSoundBuffer(*copy.m_soundBuffer);
        setLoop(copy.m_loop);

        return *this;
    }

    Sound::~Sound() {
        stop();
    }

    void Sound::play() {
        alCall(alSourcePlay, m_source);
    }

    void Sound::stop() {
        alCall(alSourceStop, m_source);
    }

    void Sound::pause() {
        alCall(alSourcePause, m_source);
    }

    void Sound::setupSource() {
        setPitch(m_pitch);
        setVolume(m_volume);
        setLoop(m_loop);

        if(m_soundBuffer)
            alCall(alSourcei, m_source, AL_BUFFER, m_soundBuffer -> getBufferID());
    }

    void Sound::setLoop(bool flag) {
        m_loop = flag;
        ALint looping = m_loop ? AL_TRUE : AL_FALSE;
        alCall(alSourcei, m_source, AL_LOOPING, looping);
    }

    bool Sound::isLooping() const {
        return m_loop;
    }

    void Sound::setSoundBuffer(const SoundBuffer& soundBuffer) {
        if(m_soundBuffer)
            stop();
        m_soundBuffer = &soundBuffer;
        setupSource();
    }

    const SoundBuffer* Sound::getSoundBuffer() const {
        return m_soundBuffer;
    }

}