#include <robot2D/Engine.hpp>
#include <game/MyApplication.hpp>

#include "audio/Sound.hpp"
#include "audio/AudioDevice.hpp"
#include "audio/Music.hpp"

int runEngine() {
    robot2D::EngineConfiguration engineConfiguration;
    engineConfiguration.windowSize = {800, 600};
    engineConfiguration.windowTitle = "Template Game";
    engineConfiguration.windowContext.renderDimensionType = robot2D::WindowContext::RenderDimensionType::TwoD;
    //engineConfiguration.windowContext.renderApi = robot2D::WindowContext::RenderApi::OpenGL3_3;
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

    while (sound.getStatus() == robot2D::SoundSource::Status::Playing) {}
}

void play_music_wav() {
    // TODO: audio context ???
    robot2D::AudioManager audioManager;
    robot2D::Music music;

    if(!music.loadFromFile("res/audio/breakout.wav"))
        return;

    music.setVolume(50.F);
    music.play();

    while(music.getStatus() == robot2D::SoundSource::Status::Playing) {}
}

int main() {
    play_music_wav();
    return 0;
}