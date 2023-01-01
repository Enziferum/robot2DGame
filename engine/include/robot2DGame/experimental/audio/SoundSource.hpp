#pragma once

#include "ALResource.hpp"

namespace robot2D {

    class SoundSource: ALResource {
    public:
        enum class Status {
            Playing = 0,
            Stopped,
            Paused
        };

        SoundSource();
        virtual ~SoundSource() = 0;
        virtual void play() = 0;
        virtual void stop() = 0;
        virtual void pause() = 0;

        void setVolume(float volume);
        float getVolume() const;

        void setPitch(float pitch) const;
        float getPitch() const;

        Status getStatus() const;
    protected:
        unsigned int ALSource;
    };

}