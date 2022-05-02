#pragma once

#include "SoundSource.hpp"
#include "SoundBuffer.hpp"

namespace robot2D {

    /// TODO copy construct and operator=
    class Sound final : public SoundSource {
    public:
        Sound();
        ~Sound() override = default;

        void setLoop(bool flag);
        bool isLooping() const;

        void play();
        void stop();
        void pause();

        void setSoundBuffer(SoundBuffer* soundBuffer);
        const SoundBuffer* getSoundBuffer() const;

    private:
        void setupSource();

    private:
        SoundBuffer* m_soundBuffer;
        bool m_loop;
    };
}
