#include <robot2D/Engine.hpp>
#include <game/MyApplication.hpp>

#include "audio/Sound.hpp"
#include "audio/AudioDevice.hpp"

int runEngine() {
    robot2D::EngineConfiguration engineConfiguration;
    engineConfiguration.windowSize = {1280, 920};
    engineConfiguration.windowTitle = "Template Game";
    ROBOT2D_RUN_ENGINE(MyApplication, engineConfiguration);
}

void play_sound_wav() {
    robot2D::AudioManager audioManager;

    std::string wav_path = "res/audio/iamtheprotectorofthissystem.wav";

    robot2D::SoundBuffer soundBuffer;
    if(!soundBuffer.loadFromFile(wav_path))
        return;

    robot2D::Sound sound;
    sound.setSoundBuffer(&soundBuffer);

    sound.play();
    std::cout << "I love you " << std::endl;

    while (sound.getStatus() == robot2D::SoundSource::Status::Playing) {}
}

int main() {
    return runEngine();
}