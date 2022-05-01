#include <robot2D/Graphics/RenderTarget.hpp>
#include <robot2D/Ecs/EntityManager.hpp>

#include <robot2DGame/systems/RendererSystem.hpp>
#include <robot2DGame/components/DrawableComponent.hpp>
#include <robot2DGame/components/TransformComponent.hpp>

namespace robot2D {
    RenderSystem::RenderSystem(robot2D::MessageBus& messageBus):
            ecs::System(messageBus,typeid(RenderSystem)) {
        addRequirement<TransformComponent>();
        addRequirement<DrawableComponent>();
    }

    void RenderSystem::update(float dt) {}

    void RenderSystem::draw(RenderTarget& target, RenderStates states) const {
        for(auto& it: m_entities) {
            auto& transform = it.getComponent<TransformComponent>();
            auto& sprite = it.getComponent<DrawableComponent>();

            auto t = transform.getTransform();
            auto sz = transform.getSize();

            t.scale(sz.x, sz.y);

            robot2D::RenderStates renderStates;
            renderStates.transform *= t;
            renderStates.texture = &sprite.getTexture();
            renderStates.color = sprite.getColor();

            target.draw(renderStates);
        }
    }
}