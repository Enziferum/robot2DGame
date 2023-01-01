#include <filesystem>

#include "robot2DGame/experimental/audio/Music.hpp"
#include "IAudioReader.hpp"
#include "WaveReader.hpp"

namespace robot2D {

    namespace fs = std::filesystem;

    std::unique_ptr<IAudioReader> getAudioReader(const fs::path&& path) {
        auto extension = path.extension();
        if(extension == ".wav")
            return std::make_unique<WaveReader>();
        return nullptr;
    }

    bool Music::loadFromFile(const std::string& path) {
        if(!m_fileInputStream.openFromFile(path))
            return false;

        std::unique_ptr<IAudioReader> audioReader = getAudioReader({path});
        if(!audioReader || !audioReader -> parseHeader(m_fileInputStream))
            return false;

        m_audioData = audioReader -> getAudioData();

        return true;
    }

    bool Music::onGetData(void* buffer, std::int32_t size) {
        std::unique_lock<std::recursive_mutex> lock{m_mutex};

        auto cursor = m_fileInputStream.tell();

        if(cursor + size > static_cast<std::int64_t>(m_audioData.audioSize)) {
            if(m_isLoop) {
                auto seekOk = m_fileInputStream.seek(0);
                if(seekOk == -1)
                    return false;
            }
            else {
                size = m_audioData.audioSize - cursor;
            }
        }

        if(!m_fileInputStream.read(buffer, size))
            return false;

        return true;
    }

}