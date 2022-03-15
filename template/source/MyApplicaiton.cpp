#include <game/MyApplication.hpp>
#include <game/States.hpp>
#include <game/IntroState.hpp>
#include <game/MenuState.hpp>
#include <game/GameState.hpp>

MyApplication::MyApplication() {}

void MyApplication::setup() {
    /// Setup here your resources ...
}

void MyApplication::registerStates() {
    /// Register Your States Here
    m_machine.registerState<GameState>(States::Intro);
    m_machine.registerState<GameState>(States::Menu);
    m_machine.registerState<GameState>(States::Game);

    m_machine.push(States::Intro);
}