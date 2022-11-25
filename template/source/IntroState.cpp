#include <robot2DGame/systems/RendererSystem.hpp>
#include <robot2DGame/systems/UIAnimationSystem.hpp>
#include <robot2DGame/systems/TextSystem.hpp>

#include <robot2DGame/components/UIAnimationComponent.hpp>
#include <robot2DGame/Api.hpp>
#include <robot2DGame/StateMachine.hpp>
#include <game/IntroState.hpp>
#include <game/States.hpp>

namespace {
    constexpr float animationDuration = 2.F;
    constexpr robot2D::vec2f animationTo = {300.F, 300.F};
}

IntroState::IntroState(robot2D::IStateMachine& machine,
                       robot2D::AppContext& context,
                       robot2D::MessageBus& messageBus):
        robot2D::State(machine, context, messageBus),
        m_scene{messageBus}
        {}

void IntroState::setup() {
    m_textures.loadFromFile(TextureID::Logo, "res/textures/logo.png");
    m_font.loadFromFile("res/font/game_font.ttf");
    setupEcs();
}

void IntroState::setupEcs() {
    auto windowSz = m_window -> getSize();
    m_scene.addSystem<robot2D::RenderSystem>(m_messageBus);
    m_scene.addSystem<robot2D::UIAnimationSystem>(m_messageBus);
    m_scene.addSystem<robot2D::TextSystem>(m_messageBus);

    robot2D::vec2f startSize = {100.F, 100.F};

    auto entity = robot2D::createEntity(m_scene,
                                        {windowSz.as<float>().x / 2.F,
                                                windowSz.as<float>().y / 2.F},
                                        startSize,
                                        m_textures.get(TextureID::Logo));

    auto& animation = entity.addComponent<robot2D::UIAnimationComponent>();
    animation.duration = robot2D::seconds(animationDuration);
    animation.from = startSize;
    animation.to = animationTo;
    animation.animationCallback = [this] {
        m_machine.pop();
        m_machine.pushState(static_cast<uint32_t>(States::Menu));
    };
    auto textPos = entity.getComponent<robot2D::TransformComponent>().getPosition();
    textPos.y += 200.F;

    robot2D::createLabel(m_scene, textPos, "Robot2D Engine", m_font);
}

void IntroState::destroy() {}

void IntroState::handleEvents(const robot2D::Event& event) {}

void IntroState::handleMessages(const robot2D::Message& message) {}

void IntroState::update(float dt) {
    m_scene.update(dt);
}

void IntroState::render() {
    m_window -> draw(m_scene);
}
