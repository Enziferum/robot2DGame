#include <robot2DGame/Api.hpp>
#include <robot2DGame/systems/UISystem.hpp>
#include <robot2DGame/systems/RendererSystem.hpp>
#include <robot2DGame/StateMachine.hpp>

#include <game/MenuState.hpp>
#include <game/States.hpp>

namespace {
    constexpr robot2D::vec2f buttonSize = {200.F, 50.F};
}

MenuState::MenuState(robot2D::IStateMachine& machine,
                     robot2D::AppContext& context,
                     robot2D::MessageBus& messageBus):
                     robot2D::State(machine, context, messageBus),
                     m_scene{messageBus}{}

void MenuState::setup() {
    m_scene.addSystem<robot2D::RenderSystem>(m_messageBus);
    m_scene.addSystem<robot2D::UISystem>(m_messageBus);
    m_textures.loadFromFile(ResourceID::Button, "res/textures/menu_button.png");

    auto windowSize = m_window -> getSize();

    auto startButton = robot2D::createImageButton(m_scene,
                                                  { windowSize.as<float>().x / 2,
                                                           windowSize.as<float>().y / 2},
                                                  buttonSize,
                                                  m_textures.get(ResourceID::Button),
                                                  BIND_BUTTON_CLASS_FN(MenuState::onStartButton));

    auto startPos = startButton.getComponent<robot2D::TransformComponent>().getPosition();
    robot2D::vec2f endBtnPosition = {startPos.x, startPos.y + buttonSize.y * 2};

    auto endButton = robot2D::createImageButton(m_scene,
                                                endBtnPosition,
                                                buttonSize,
                                                  m_textures.get(ResourceID::Button),
                                                  BIND_BUTTON_CLASS_FN(MenuState::onEndButton));
}

void MenuState::onStartButton(robot2D::ecs::Entity entity, uint64_t flags) {
    (void)entity;

    if ( flags & robot2D::UISystem::Flags::LeftMouse) {
        m_machine.pop();
        m_machine.pushState(static_cast<robot2D::StateID>(States::Game));
    }
}

void MenuState::onEndButton(robot2D::ecs::Entity entity, uint64_t flags) {
    (void)entity;
    if ( flags & robot2D::UISystem::Flags::LeftMouse)
        destroy();
}

void MenuState::destroy() {
    // TODO m_machine.close() not window directly
    m_window -> close();
}

void MenuState::handleEvents(const robot2D::Event& event) {
    m_scene.getSystem<robot2D::UISystem>() -> handleEvents(event);
}

void MenuState::handleMessages(const robot2D::Message& message) {

}

void MenuState::update(float dt) {
    m_scene.update(dt);
}

void MenuState::render() {
    m_window -> draw(m_scene);
}
