#include <cassert>
#include <memory>
#include <cstring>
#include "robot2DGame/experimental/audio/SoundStream.hpp"
#include "robot2DGame/experimental/audio/ALHelpers.hpp"

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
        using Ptr = std::shared_ptr<ALBuffer>;

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


    SoundStream::SoundStream(): m_streaming{false} {}

    SoundStream::~SoundStream() {
        awaitStreamingThread();
    }

    void SoundStream::play() {
        {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
        }

       // stop();
        launchStreamThread();
    }

    void SoundStream::stop() {
       awaitStreamingThread();
    }

    void SoundStream::pause() {
        {
            // TODO threading stuff
        }
        {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);

            if (!m_streaming)
                return;

         //   m_threadStartState = Paused;
        }
        alCall(alSourcePause, m_source);
    }

    void SoundStream::initalize() {

    }

    void SoundStream::launchStreamThread() {
        {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            m_streaming = true;
        }
        auto audioData = getAudioData();
        m_thread = std::thread(&SoundStream::streamThread, this, audioData);
    }

    void SoundStream::streamThread(AudioData audioData) {
        bool requestStop = false;
        ALBuffer::Ptr buffers[bufferMax];

        assert(audioData.audioSize >= bufferSize * bufferMax);
        char audioBuffer[bufferMax * bufferSize];

        std::memset(audioBuffer, 0, bufferMax * bufferSize);

        /// Prepack first buffers ///
        for(int i = 0; i < bufferMax; ++i) {
            buffers[i] = std::make_shared<ALBuffer>(audioData.audioFormatType, audioData.sampleRate);
            onGetData(&audioBuffer[i * bufferSize], bufferSize);
            buffers[i] -> setData(&audioBuffer[i * bufferSize], bufferSize);
            auto bufferID = buffers[i] -> getID();
            alSourceQueueBuffers(m_source, 1, &bufferID);
        }

        std::memset(audioBuffer, 0, bufferMax * bufferSize);
        alCall(alSourcePlay, m_source);

        for(;;) {
            {
                std::unique_lock<std::recursive_mutex> lock{m_mutex};
                if(!m_streaming)
                    break;
            }

            if(SoundSource::getStatus() == SoundSource::Status::Stopped) {
                if(!requestStop){

                }
                else {
                    std::unique_lock<std::recursive_mutex> lock{m_mutex};
                    m_streaming = false;
                }

            }

            ALint buffersProcessed = 0;
            alCall(alGetSourcei, m_source, AL_BUFFERS_PROCESSED, &buffersProcessed);

            while(buffersProcessed--) {
                ALuint processBuffer;
                alSourceUnqueueBuffers(m_source, 1, &processBuffer);

                ALBuffer::Ptr curr = nullptr;

                int index = 0;
                for(; index < bufferMax; ++index) {
                    if(processBuffer == buffers[index] -> getID()) {
                        curr = buffers[index];
                        break;
                    }
                }

                if(!onGetData(&audioBuffer[index * bufferSize], bufferSize)) {
                    /// error or end
                    requestStop = true;
                }

                curr -> setData(&audioBuffer[index * bufferSize], bufferSize);
                alSourceQueueBuffers(m_source, 1, &processBuffer);
            }

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(10ms);
        }

        alCall(alSourceStop, m_source);
        clearQueue();
    }

    void SoundStream::clearQueue() {
        ALint nbQueued;
        alCall(alGetSourcei, m_source, AL_BUFFERS_QUEUED, &nbQueued);

        // Dequeue them all
        ALuint buffer;
        for (ALint i = 0; i < nbQueued; ++i)
            alCall(alSourceUnqueueBuffers, m_source, 1, &buffer);
    }

    void SoundStream::awaitStreamingThread() {
        {
            std::unique_lock<std::recursive_mutex> lock{m_mutex};
            m_streaming = false;
        }

        if(m_thread.joinable())
            m_thread.join();
    }

}