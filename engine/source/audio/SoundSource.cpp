#include <cassert>
#include "robot2DGame/experimental/audio/ALHelpers.hpp"
#include "robot2DGame/experimental/audio/SoundSource.hpp"

namespace robot2D {

    SoundSource::SoundSource() {
        alCall(alGenSources, 1, &m_source);
        alCall(alSourcei, m_source, AL_BUFFER, 0);

        // only just now
        alCall(alSource3f, m_source, AL_POSITION, 0, 0, 0);
        alCall(alSource3f, m_source, AL_VELOCITY, 0, 0, 0);
    }

    SoundSource::SoundSource(const SoundSource& copy) {
        alCall(alGenSources, 1, &m_source);
        alCall(alSourcei, m_source, AL_BUFFER, 0);

        setVolume(copy.getVolume());
        setPitch(copy.getPitch());

        // only just now
        alCall(alSource3f, m_source, AL_POSITION, 0, 0, 0);
        alCall(alSource3f, m_source, AL_VELOCITY, 0, 0, 0);
    }

    SoundSource& SoundSource::operator=(const SoundSource& copy) {
        setVolume(copy.getVolume());
        setPitch(copy.getPitch());

        // only just now
        alCall(alSource3f, m_source, AL_POSITION, 0, 0, 0);
        alCall(alSource3f, m_source, AL_VELOCITY, 0, 0, 0);
        return *this;
    }

    SoundSource::~SoundSource() {
        alCall(alSourcei, m_source, AL_BUFFER, 0);
        alCall(alDeleteSources, 1, &m_source);
    }

    void SoundSource::setVolume(float volume) {
        assert((volume >= 0.F && volume <= 100.F));
        if(volume > 1.0F)
            volume /= 100.F;
        alCall(alSourcef, m_source, AL_GAIN, volume);
        m_volume = volume;
    }

    float SoundSource::getVolume() const {
        float gain = 0.F;
        alCall(alGetSourcef, m_source, AL_GAIN, &gain);
        return gain * 100.F;
    }

    void SoundSource::setPitch(float pitch) const {
        alCall(alSourcef, m_source, AL_PITCH, pitch);
    }

    float SoundSource::getPitch() const {
        float pitch = 0.F;
        alCall(alGetSourcef, m_source, AL_GAIN, &pitch);
        return pitch;
    }

    SoundSource::Status SoundSource::getStatus() const {
        ALint rawStatus;
        alCall(alGetSourcei, m_source, AL_SOURCE_STATE, &rawStatus);

        Status status = Status::None;
        switch(rawStatus) {
            default:
                break;
            case AL_PLAYING:
                status = Status::Playing;
                break;
            case AL_INITIAL:
            case AL_STOPPED:
                status = Status::Stopped;
                break;
            case AL_PAUSED:
                status = Status::Paused;
                break;
        }
        return status;
    }



}
