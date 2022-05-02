#pragma once

#include "AudioData.hpp"

namespace robot2D {

    class IAudioReader {
    public:
        virtual ~IAudioReader() = 0;
        virtual bool loadFromFile(const std::string& path) = 0;

        const AudioData& getAudioData() const {
            return m_audioData;
        }
    protected:
        AudioData m_audioData;
    };

}