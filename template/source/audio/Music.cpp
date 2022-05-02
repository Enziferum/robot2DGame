#include "Music.hpp"

namespace robot2D {

    Music::Music() {

    }

    Music::~Music() {
        stop();
    }

    bool Music::loadFromFile(const std::string& path) {

        initalize();
        return true;
    }

    void Music::initalize() {
        launchThread();
    }

    void Music::launchThread() {

    }


}