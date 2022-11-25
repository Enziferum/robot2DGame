#pragma once

#include <thread>
#include <mutex>

#include "SoundSource.hpp"
#include "AudioData.hpp"

namespace robot2D {

    class SoundStream: public SoundSource {
    public:
        SoundStream();
        virtual ~SoundStream();

        void play() override;
        void stop() override;
        void pause() override;

    protected:
        void initalize();
        void streamThread(AudioData audioData);
        void launchStreamThread();
        virtual bool onGetData(char* buffer, std::int32_t size) = 0;
        virtual const AudioData& getAudioData() const = 0;
    private:

        void clearQueue();

    protected:
        std::recursive_mutex m_mutex;
    private:
        std::thread m_thread;
    };

}


