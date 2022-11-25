#include "Music.hpp"

namespace robot2D {

    Music::Music() {}

    bool Music::loadFromFile(const std::string& path) {
        if(!m_fileInputStream.openFromFile(path))
            return false;

        // TODO: add Reader

        return true;
    }

    bool Music::onGetData(char* buffer, std::int32_t size) {
        std::unique_lock<std::recursive_mutex> lock{m_mutex};

        auto cursor = m_fileInputStream.tell();

        if(!m_fileInputStream.read(buffer, size))
            return false;

        return true;
    }

}