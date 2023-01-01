#include <iostream>

#include "robot2DGame/experimental/audio/ALHelpers.hpp"
#include "robot2DGame/experimental/audio/SoundBuffer.hpp"
#include "WaveReader.hpp"

namespace robot2D {

    SoundBuffer::SoundBuffer() {}

    SoundBuffer::~SoundBuffer() {
        alCall(alDeleteBuffers, 1, &ALBO);
    }

    bool SoundBuffer::loadFromFile(const std::string& path) {

        std::unique_ptr<IAudioReader> reader = std::make_unique<WaveReader>();
        if(!reader)
            return false;

        if(!reader -> loadFromFile(path, m_soundData))
            return false;

        m_audioData = reader -> getAudioData();

        setupBuffer();

        return true;
    }

    void SoundBuffer::setupBuffer() {
        alCall(alGenBuffers, 1, &ALBO);

        ALenum format;

        if (m_audioData.channels == 1 && m_audioData.bitsPerSample == 8)
            format = AL_FORMAT_MONO8;
        else if (m_audioData.channels == 1 && m_audioData.bitsPerSample == 16)
            format = AL_FORMAT_MONO16;
        else if (m_audioData.channels == 2 && m_audioData.bitsPerSample == 8)
            format = AL_FORMAT_STEREO8;
        else if (m_audioData.channels == 2 && m_audioData.bitsPerSample == 16)
            format = AL_FORMAT_STEREO16;
        else {
            std::cerr
                    << "ERROR: unrecognised wave format: "
                    << m_audioData.channels << " channels, "
                    << m_audioData.bitsPerSample << " bps" << std::endl;
            return;
        }

        // pack buffer //
        alCall(alBufferData,
               ALBO,
               format,
               m_soundData.data(), m_soundData.size(),
               m_audioData.sampleRate);

        m_soundData.clear();
    }

}