#include <robot2D/robot2D.hpp>
#include <game/MyApplication.hpp>

int main() {
    robot2D::InitializeEngine();

    MyApplication application;
    application.setup();

    try {
        application.run();
    }
    catch (...) {

    }
    return 0;
}