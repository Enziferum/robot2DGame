#include <iostream>
#include "AudioDevice.hpp"

namespace robot2D {
    AudioDevice::AudioDevice() {
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

    AudioDevice::~AudioDevice() {
        ALCboolean closed;

        alcCall(alcMakeContextCurrent, contextMadeCurrent, openALDevice, nullptr);
        if(openALContext)
            alcCall(alcDestroyContext, openALDevice, openALContext);
        if(openALDevice)
            alcCall(alcCloseDevice, closed, openALDevice, openALDevice);
    }
}