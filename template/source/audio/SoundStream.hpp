#pragma once

#include "SoundSource.hpp"

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
    };

}


