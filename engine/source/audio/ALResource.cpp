#include <memory>
#include <mutex>

#include <robot2DGame/experimental/audio/ALResource.hpp>
#include "AudioDevice.hpp"

namespace {
    std::recursive_mutex mutex;
    unsigned int g_count = 0;
    std::unique_ptr<robot2D::AudioDevice> globalDevice{nullptr};
}

namespace robot2D {
    ALResource::ALResource() {
        std::unique_lock<std::recursive_mutex> lock{mutex};
        if(g_count == 0)
            globalDevice = std::make_unique<AudioDevice>();
        ++g_count;
    }

    ALResource::~ALResource() {
        std::unique_lock<std::recursive_mutex> lock{mutex};
        --g_count;
        if(g_count == 0)
            globalDevice.reset();
    }
}