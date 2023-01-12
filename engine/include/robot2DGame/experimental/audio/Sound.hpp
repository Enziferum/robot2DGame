#pragma once

#include "SoundSource.hpp"
#include "SoundBuffer.hpp"

namespace robot2D {

    /// TODO copy construct and operator=
    class Sound final : public SoundSource {
    public:
        Sound();
        Sound(const Sound& copy);
        Sound& operator=(const Sound& copy);

        Sound(const SoundBuffer* soundBuffer);
        ~Sound() override;

        void setLoop(bool flag);
        bool isLooping() const;

        void play() override;
        void stop() override;
        void pause() override;

        void setSoundBuffer(const SoundBuffer& soundBuffer);
        const SoundBuffer* getSoundBuffer() const;

    private:
        void setupSource();

    private:
        const SoundBuffer* m_soundBuffer;
        bool m_loop;
    };
}
