#pragma once

#include <robot2DGame/experimental/audio/ALHelpers.hpp>

namespace robot2D {

    class AudioDevice {
    public:
        AudioDevice();
        ~AudioDevice();

    private:
        ALCdevice* openALDevice;
        ALCcontext* openALContext;
        ALCboolean contextMadeCurrent;
    };
}
