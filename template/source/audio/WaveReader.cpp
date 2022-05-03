#include <iostream>
#include <cstring>
#include "WaveReader.hpp"

namespace robot2D {

    inline std::int32_t read32int(const char* buffer, std::size_t len) {
        std::int32_t res;
        memcpy(&res, buffer, len);
        return res;
    }

    WaveReader::WaveReader() {}

    bool WaveReader::checkHeader(std::ifstream &file, unsigned int &size) {

        if (!file.is_open())
            return false;

        char buffer[4];

        if (!file.read(buffer, 4))
            return false;

        if (std::strncmp(buffer, "RIFF", 4) != 0)
            return false;

        // the size of the file
        if (!file.read(buffer, 4)) {
            std::cerr << "ERROR: could not read size of file" << std::endl;
            return false;
        }

        // the WAVE
        if (!file.read(buffer, 4)) {
            std::cerr << "ERROR: could not read WAVE" << std::endl;
            return false;
        }
        if (std::strncmp(buffer, "WAVE", 4) != 0) {
            std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
            return false;
        }

        // "fmt/0"
        if (!file.read(buffer, 4)) {
            std::cerr << "ERROR: could not read fmt/0" << std::endl;
            return false;
        }

        // this is always 16, the size of the fmt data chunk
        if (!file.read(buffer, 4)) {
            std::cerr << "ERROR: could not read the 16" << std::endl;
            return false;
        }

        // PCM should be 1?
        if (!file.read(buffer, 2)) {
            std::cerr << "ERROR: could not read PCM" << std::endl;
            return false;
        }

        // the number of channels
        if (!file.read(buffer, 2)) {
            std::cerr << "ERROR: could not read number of channels" << std::endl;
            return false;
        }
        m_audioData.channels = read32int(buffer, 2);

        // sample rate
        if (!file.read(buffer, 4)) {
            std::cerr << "ERROR: could not read sample rate" << std::endl;
            return false;
        }
        m_audioData.sampleRate = read32int(buffer, 4);

        // (sampleRate * bitsPerSample * channels) / 8
        if (!file.read(buffer, 4)) {
            std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
            return false;
        }

        // ?? dafaq
        if (!file.read(buffer, 2)) {
            std::cerr << "ERROR: could not read dafaq" << std::endl;
            return false;
        }

        // bitsPerSample
        if (!file.read(buffer, 2)) {
            std::cerr << "ERROR: could not read bits per sample" << std::endl;
            return false;
        }
        m_audioData.bitsPerSample = read32int(buffer, 2);

        // data chunk header "data"
        if (!file.read(buffer, 4)) {
            std::cerr << "ERROR: could not read data chunk header" << std::endl;
            return false;
        }
        if (std::strncmp(buffer, "data", 4) != 0) {
            std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
            return false;
        }

        // size of data
        if (!file.read(buffer, 4)) {
            std::cerr << "ERROR: could not read data size" << std::endl;
            return false;
        }
        size = read32int(buffer, 4);

        /* cannot be at the end of file */
        if (file.eof()) {
            std::cerr << "ERROR: reached EOF on the file" << std::endl;
            return false;
        }
        if (file.fail()) {
            std::cerr << "ERROR: fail state set on the file" << std::endl;
            return false;
        }

        return true;
    }

    bool WaveReader::loadFromFile(const std::string& path) {
        std::ifstream file{path, std::ios::binary};

        if (!file.is_open())
            return false;

        unsigned int size;

        if (!checkHeader(file, size))
            return false;

        m_audioData.soundData.resize(size);
        file.read(m_audioData.soundData.data(), size);
        return true;
    }

}