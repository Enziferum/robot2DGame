#pragma once
#include <string>
#include <vector>

#include <robot2D/Core/FileInputStream.hpp>
#include "robot2DGame/experimental/audio/AudioData.hpp"

namespace robot2D {

    class IAudioReader {
    public:
        virtual ~IAudioReader() = 0;

        virtual bool loadFromFile(const std::string& path, std::vector<char>& buffer) = 0;
//        virtual bool openFromFile(const std::string& path,
//                                  const FileInputStream& fileInputStream) = 0;

        virtual bool parseHeader(FileInputStream& fileInputStream) = 0;
        const AudioData& getAudioData() const { return m_audioData; }
    protected:
        AudioData m_audioData;
        FileInputStream* m_stream{nullptr};
    };

}