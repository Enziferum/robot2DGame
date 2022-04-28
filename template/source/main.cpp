#include <robot2D/Engine.hpp>
#include <game/MyApplication.hpp>

int main() {
    robot2D::EngineConfiguration engineConfiguration;
    engineConfiguration.windowSize = {1280, 920};
    engineConfiguration.windowTitle = "Template Game";
    ROBOT2D_RUN_ENGINE(MyApplication, engineConfiguration);
}