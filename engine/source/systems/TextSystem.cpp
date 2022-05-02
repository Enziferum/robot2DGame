#include <robot2D/Ecs/EntityManager.hpp>

#include <robot2DGame/systems/TextSystem.hpp>
#include <robot2DGame/components/DrawableComponent.hpp>
#include <robot2DGame/components/TextComponent.hpp>

namespace robot2D {
    TextSystem::TextSystem(robot2D::MessageBus& messageBus):
        robot2D::ecs::System(messageBus, typeid(TextSystem)) {
        addRequirement<TextComponent>();
    }

    void TextSystem::update(float dt) {
        (void)dt;
        for(auto& entites: m_entities) {

        }
    }
}