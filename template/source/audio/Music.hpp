#pragma once

#include <thread>
#include <mutex>

#include "SoundStream.hpp"

namespace robot2D {
    class Music: public robot2D::SoundStream {
    public:
        Music();
        ~Music();

        bool loadFromFile(const std::string& path);
    private:
        void initalize();
        void launchThread();

    private:
        std::thread m_thread;
        std::recursive_mutex m_recursiveMutex;
    };
}