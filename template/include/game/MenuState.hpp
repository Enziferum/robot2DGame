#pragma once

#include <robot2D/Core/MessageBus.hpp>
#include <robot2D/Ecs/Scene.hpp>
#include <robot2D/Util/ResourceHandler.hpp>

#include <robot2DGame/State.hpp>
#include <robot2DGame/Font.hpp>

enum class ResourceID {
    Button,
    Font
};

class MenuState final: public robot2D::State {
public:
    MenuState(robot2D::IStateMachine& machine,
              robot2D::AppContext& context,
              robot2D::MessageBus& messageBus);
    ~MenuState() override = default;

    void setup() override;
    void destroy() override;

    void handleEvents(const robot2D::Event& ) override;
    void handleMessages(const robot2D::Message&) override;
    void update(float dt) override;
    void render() override;

private:
    void onStartButton(robot2D::ecs::Entity entity, uint64_t flags);
    void onEndButton(robot2D::ecs::Entity entity, uint64_t flags);
private:
    robot2D::ecs::Scene m_scene;
    robot2D::ResourceHandler<robot2D::Texture, ResourceID> m_textures;
    robot2D::ResourceHandler<robot2D::Font, ResourceID> m_fonts;
};