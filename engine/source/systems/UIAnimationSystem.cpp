#include <robot2DGame/systems/UIAnimationSystem.hpp>
#include <robot2DGame/components/TransformComponent.hpp>
#include <robot2DGame/components/UIAnimationComponent.hpp>

#include <robot2D/Ecs/EntityManager.hpp>

namespace robot2D {

    UIAnimationSystem::UIAnimationSystem(MessageBus& bus):
            robot2D::ecs::System(bus, typeid(UIAnimationSystem)) {
        addRequirement<TransformComponent>();
        addRequirement<UIAnimationComponent>();
    }

    void UIAnimationSystem::update(float dt) {

        for(auto& it: m_entities) {
            auto& animation = it.getComponent<UIAnimationComponent>();
            auto& transform = it.getComponent<TransformComponent>();

            if(animation.duration.asMicroSeconds() > 0) {
                auto t = seconds(dt);
                if(animation.type == UIAnimationComponent::AnimationType::Scale) {
                    auto newSize = animation.getValue(dt);
                    auto oldSize = transform.getSize();
                    transform.setSizeScale(newSize);

                    auto currPos = transform.getPosition();
                    currPos.x -= (newSize.x - oldSize.x) / 2.F;
                    currPos.y -= (newSize.y - oldSize.y) / 2.F;

                    transform.setPosition(currPos);
                }
                animation.duration -= t;
            } else {
                animation.onCallback();
            }
        }
    }
}