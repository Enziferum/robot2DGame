#include "ALHelpers.hpp"
#include "SoundSource.hpp"

namespace robot2D {

    SoundSource::SoundSource() {
        alCall(alGenSources, 1, &ALSource);
        alCall(alSourcei, ALSource, AL_BUFFER, 0);

        // only just now
        alCall(alSource3f, ALSource, AL_POSITION, 0, 0, 0);
        alCall(alSource3f, ALSource, AL_VELOCITY, 0, 0, 0);
    }

    SoundSource::~SoundSource() {
        alCall(alSourcei, ALSource, AL_BUFFER, 0);
        alCall(alDeleteSources, 1, &ALSource);
    }

    void SoundSource::setVolume(float volume) {
        alCall(alSourcef, ALSource, AL_GAIN, volume);
    }

    float SoundSource::getVolume() const {
        float gain = 0.F;
        alCall(alGetSourcef, ALSource, AL_GAIN, &gain);
        // gain from 0 to 1
        return gain * 100.F;
    }

    void SoundSource::setPitch(float pitch) const {
        alCall(alSourcef, ALSource, AL_PITCH, pitch);
    }

    float SoundSource::getPitch() const {
        float pitch = 0.F;
        alCall(alGetSourcef, ALSource, AL_GAIN, &pitch);
        return pitch;
    }

    SoundSource::Status SoundSource::getStatus() const {
        ALint rawStatus;
        alCall(alGetSourcei, ALSource, AL_SOURCE_STATE, &rawStatus);

        Status status;
        switch(rawStatus) {
            case AL_PLAYING:
                status = Status::Playing;
                break;
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
