#pragma once

#include <string>
#include "AudioData.hpp"

namespace robot2D {
    using BufferID = unsigned int;

    class SoundBuffer {
    public:
        SoundBuffer();
        ~SoundBuffer();

        bool loadFromFile(const std::string &path);
        BufferID getBufferID() const { return ALBO; }

    private:
        void setupBuffer();

    private:
        AudioData m_audioData;
        BufferID ALBO;
    };
}
