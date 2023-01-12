#include <iostream>
#include <cstring>
#include <filesystem>

#include "WaveReader.hpp"

namespace robot2D {
    constexpr int waveHeaderSize = 44;

    inline std::int32_t read32int(const char* buffer, std::size_t len) {
        std::int32_t res;
        memcpy(&res, buffer, len);
        return res;
    }

    WaveReader::WaveReader() {}

    bool WaveReader::checkHeader(std::ifstream& file, unsigned int& size) {

        if (!file.is_open())
            return false;

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

    bool WaveReader::loadFromFile(const std::string& path, std::vector<char>& buffer) {
        std::ifstream file{path, std::ios::binary};

        if (!file.is_open())
            return false;

        char headerBuffer[waveHeaderSize];
        file.read(&headerBuffer[0], waveHeaderSize);

        m_audioData.channels = read32int(&headerBuffer[22], 2);

        // sample rate
        m_audioData.sampleRate = read32int(&headerBuffer[24], 4);

        // (sampleRate * bitsPerSample * channels) / 8


        m_audioData.bitsPerSample = read32int(&headerBuffer[34], 2);

        if (std::strncmp(&headerBuffer[36], "data", 4) == 0) {
            m_audioData.audioSize = read32int(&headerBuffer[40], 4);
        }
        else {
            auto fileSize = std::filesystem::file_size(path);
            m_audioData.audioSize = fileSize;
        }

        if(m_audioData.channels == 1 && m_audioData.bitsPerSample == 8)
            m_audioData.audioFormatType = AudioFormatType::MONO8;
        else if(m_audioData.channels == 1 && m_audioData.bitsPerSample == 16)
            m_audioData.audioFormatType = AudioFormatType::MONO16;
        else if(m_audioData.channels == 2 && m_audioData.bitsPerSample == 8)
            m_audioData.audioFormatType = AudioFormatType::STEREO8;
        else if(m_audioData.channels == 2 && m_audioData.bitsPerSample == 16)
            m_audioData.audioFormatType = AudioFormatType::STEREO16;
        else
        {
            std::cerr
                    << "ERROR: unrecognised wave format: "
                    << m_audioData.channels << " channels, "
                    << m_audioData.bitsPerSample << " bps" << std::endl;
            return false;
        }

        buffer.resize(m_audioData.audioSize);
        file.read(&buffer[0], m_audioData.audioSize);


        return true;
    }

    bool WaveReader::parseHeader(FileInputStream& fileInputStream) {
        m_stream = &fileInputStream;


        char buffer[waveHeaderSize];

        if(!m_stream -> read(&buffer[0], waveHeaderSize))
            return false;

        if (std::strncmp(&buffer[0], "RIFF", 4) != 0)
            return false;

        if (std::strncmp(&buffer[8], "WAVE", 4) != 0) {
            std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
            return false;
        }

        // "fmt/0"

        // PCM should be 1?

        // the number of channels
        m_audioData.channels = read32int(&buffer[22], 2);

        // sample rate
        m_audioData.sampleRate = read32int(&buffer[24], 4);

        // (sampleRate * bitsPerSample * channels) / 8


        m_audioData.bitsPerSample = read32int(&buffer[34], 2);

        // data chunk header "data"
//        if (std::strncmp(&buffer[36], "data", 4) != 0) {
//            std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
//            return false;
//        }

        // size of data
       // m_audioData.audioSize = read32int(&buffer[40], 4);

       m_audioData.audioSize = m_stream -> getSize() - waveHeaderSize;

        if(m_audioData.channels == 1 && m_audioData.bitsPerSample == 8)
            m_audioData.audioFormatType = AudioFormatType::MONO8;
        else if(m_audioData.channels == 1 && m_audioData.bitsPerSample == 16)
            m_audioData.audioFormatType = AudioFormatType::MONO16;
        else if(m_audioData.channels == 2 && m_audioData.bitsPerSample == 8)
            m_audioData.audioFormatType = AudioFormatType::STEREO8;
        else if(m_audioData.channels == 2 && m_audioData.bitsPerSample == 16)
            m_audioData.audioFormatType = AudioFormatType::STEREO16;
        else
        {
            std::cerr
                    << "ERROR: unrecognised wave format: "
                    << m_audioData.channels << " channels, "
                    << m_audioData.bitsPerSample << " bps" << std::endl;
            return false;
        }


        return true;
    }



}