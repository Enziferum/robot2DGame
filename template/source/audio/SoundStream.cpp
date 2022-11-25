#include <cassert>
#include <memory>
#include "SoundStream.hpp"
#include "ALHelpers.hpp"

namespace robot2D {
    namespace {
        constexpr int bufferSize = 65536; // 32kb
        constexpr int bufferMax = 4;
    }

    ALenum convertAudioFormat(const AudioFormatType& audioFormatType) {
        switch(audioFormatType) {
            case AudioFormatType::MONO8:
                return AL_FORMAT_MONO8;
            case AudioFormatType::MONO16:
                return AL_FORMAT_MONO16;
            case AudioFormatType::STEREO8:
                return AL_FORMAT_STEREO8;
            case AudioFormatType::STEREO16:
                return AL_FORMAT_STEREO16;
        }
    }

    struct ALBuffer {
        ALBuffer(AudioFormatType format, std::int32_t sampleRate);
        ~ALBuffer();

        ALuint getID() const noexcept { return m_bufferID;}
        void setData(void* buffer, std::int32_t size);
    private:
        ALuint m_bufferID{0};
        ALenum m_format{0};
        std::int32_t m_sampleRate{0};
    };

    ALBuffer::ALBuffer(robot2D::AudioFormatType format, std::int32_t sampleRate) {
        alCall(alGenBuffers, 1, &m_bufferID);
        m_format = convertAudioFormat(format);
        m_sampleRate = sampleRate;
    }

    ALBuffer::~ALBuffer() {
        alCall(alDeleteBuffers, 1, &m_bufferID);
    }

    void ALBuffer::setData(void* buffer, std::int32_t size) {
        alCall(alBufferData, m_bufferID, m_format, buffer,
               static_cast<ALsizei>(size), static_cast<ALsizei>(m_sampleRate));
    }

    SoundStream::SoundStream() {}

    SoundStream::~SoundStream() {
        if(m_thread.joinable())
            m_thread.join();
    }

    void SoundStream::play() {
        stop();
        launchStreamThread();
    }

    void SoundStream::stop() {
        {
            // TODO threading stuff
        }
        alCall(alSourcei, ALSource, AL_STOPPED, true);
    }

    void SoundStream::pause() {
        {
            // TODO threading stuff
        }
        alCall(alSourcei, ALSource, AL_PAUSED, true);
    }

    void SoundStream::initalize() {

    }

    void SoundStream::launchStreamThread() {
        auto audioData = getAudioData();
        m_thread = std::thread(&SoundStream::streamThread, this, audioData);
    }

    void SoundStream::streamThread(AudioData audioData) {
        std::shared_ptr<ALBuffer> buffers[bufferMax];

        assert(audioData.audioSize >= bufferSize * bufferMax);
        char audioBuffer[bufferMax * bufferSize];

        /// Prepack first buffers ///
        for(int i = 0; i < bufferMax; ++i) {
            buffers[i] = std::make_shared<ALBuffer>(audioData.audioFormatType, audioData.sampleRate);
            onGetData(&audioBuffer[i * bufferSize], bufferSize);
            buffers[i] -> setData(&audioBuffer[i * bufferSize], bufferSize);
            auto bufferID = buffers[i] -> getID();
            alSourceQueueBuffers(ALSource, 1, &bufferID);
        }

        alCall(alSourcei, ALSource, AL_PLAYING, true);

        for(;;) {
            ALint buffersProcessed = 0;
            alSourcei(ALSource, AL_BUFFERS_PROCESSED, buffersProcessed);

            while(buffersProcessed--) {
                auto bufferID = buffers[buffersProcessed] -> getID();
                alSourceUnqueueBuffers(ALSource, 1, &bufferID);

                if(!onGetData(&audioBuffer[buffersProcessed * bufferSize], bufferSize)) {
                    /// error or end
                }

                buffers[buffersProcessed] ->
                    setData(&audioBuffer[buffersProcessed * bufferSize], bufferSize);
                alSourceQueueBuffers(ALSource, 1, &bufferID);
            }

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(10ms);
        }

        clearQueue();
    }

    void SoundStream::clearQueue() {
        /// TODO unqueue buffers ///
    }

}