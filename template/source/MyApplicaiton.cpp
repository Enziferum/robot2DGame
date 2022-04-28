#include <game/MyApplication.hpp>

#include <game/States.hpp>
#include <game/IntroState.hpp>
#include <game/MenuState.hpp>
#include <game/GameState.hpp>

MyApplication::MyApplication():
    robot2D::GameApplication() {}

void MyApplication::setup() {
    /// Setup here your resources ...
    GameApplication::setup();
    registerStates();
}

void MyApplication::registerStates() {
    /// Register Your States Here
    m_machine.registerState<IntroState>(static_cast<robot2D::StateID>(States::Intro));
    m_machine.registerState<MenuState>(static_cast<robot2D::StateID>(States::Menu));
    m_machine.registerState<GameState>(static_cast<robot2D::StateID>(States::Game));

    m_machine.pushState(static_cast<robot2D::StateID>(States::Intro));
}

void MyApplication::handleEvents(const robot2D::Event& event) {
    m_machine.handleEvent(event);
}

void MyApplication::update(float dt) {
    m_machine.update(dt);
}

void MyApplication::render() {
    m_window -> clear();
    m_window -> beforeRender();
    m_machine.render();
    m_window -> afterRender();
    m_window -> display();
}
