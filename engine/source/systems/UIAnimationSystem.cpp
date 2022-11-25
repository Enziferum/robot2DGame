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
                    transform.setSizeScale(animation.getValue(dt));
                }
                animation.duration -= t;
            } else {
                animation.onCallback();
            }
        }
    }
}