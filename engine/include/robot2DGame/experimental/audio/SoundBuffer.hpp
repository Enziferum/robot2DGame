#pragma once

#include <memory>
#include <string>

#include "AudioData.hpp"

namespace robot2D {
    using BufferID = unsigned int;

    class SoundBuffer {
    public:
        SoundBuffer();
        ~SoundBuffer();

        bool loadFromFile(const std::string& path, AudioFormatType audioFormatType = AudioFormatType::None);
        BufferID getBufferID() const { return ALBO; }

    private:
        void setupBuffer(AudioFormatType audioFormatType = AudioFormatType::None);

    private:
        AudioData m_audioData;
        BufferID ALBO;
        std::vector<char> m_soundData;
    };
}
