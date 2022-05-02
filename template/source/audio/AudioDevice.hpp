#pragma once

#include <iostream>
#include "ALHelpers.hpp"

namespace robot2D {

    class AudioManager {
    public:
        AudioManager();
        ~AudioManager();

    private:
        ALCdevice* openALDevice;
        ALCcontext* openALContext;
        ALCboolean contextMadeCurrent;
    };

    AudioManager::AudioManager() {

        openALDevice = alcOpenDevice(nullptr);
        if (!openALDevice)
            return;

        if (!alcCall(alcCreateContext, openALContext, openALDevice, openALDevice, nullptr) || !openALContext) {
            std::cerr << "ERROR: Could not create audio context" << std::endl;
        }

        contextMadeCurrent = false;
        if (!alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, openALContext)
            || contextMadeCurrent != ALC_TRUE) {
            std::cerr << "ERROR: Could not make audio context current" << std::endl;
        }
    }

    AudioManager::~AudioManager() {
        ALCboolean closed;
        alcCall(alcCloseDevice, closed, openALDevice, openALDevice);

        alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, nullptr);
        alcCall(alcDestroyContext, openALDevice, openALContext);
    }

}
