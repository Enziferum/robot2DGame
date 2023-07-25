#pragma once

#include <robot2DGame/State.hpp>
#include <robot2D/Ecs/Scene.hpp>
#include <robot2D/Util/ResourceHandler.hpp>
#include <robot2D/Graphics/Font.hpp>

enum class TextureID {
    Logo = 0
};

class IntroState final: public robot2D::State {
public:
    IntroState(robot2D::IStateMachine& machine,
               robot2D::AppContext& context,
               robot2D::MessageBus& messageBus);
    ~IntroState() override = default;

    void setup() override;
    void destroy() override;

    void handleEvents(const robot2D::Event&) override;
    void handleMessages(const robot2D::Message&) override;

    void update(float dt) override;
    void render() override;

private:
    void setupEcs();
private:
    robot2D::ecs::Scene m_scene;
    robot2D::ResourceHandler<robot2D::Texture, TextureID> m_textures;
    robot2D::Font m_font;
};