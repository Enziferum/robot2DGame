#include <robot2DGame/systems/UISystem.hpp>
#include <robot2DGame/components/TransformComponent.hpp>
#include <robot2DGame/components/UIHitBox.hpp>

namespace robot2D {

    UISystem::UISystem(robot2D::MessageBus& bus):
            robot2D::ecs::System{bus, typeid(UISystem)} {
        addRequirement<TransformComponent>();
        addRequirement<UIHitbox>();
    }

    void UISystem::handleEvents(const robot2D::Event& event) {

        if(event.type == robot2D::Event::MousePressed) {

            if(event.mouse.btn == robot2D::Mouse::MouseLeft)
                m_mouseDownEvents.emplace_back(Flags::LeftMouse);
            if(event.mouse.btn == robot2D::Mouse::MouseRight)
                m_mouseDownEvents.emplace_back(Flags::RightMouse);
            if(event.mouse.btn == robot2D::Mouse::MouseMiddle)
                m_mouseDownEvents.emplace_back(Flags::MiddleMouse);

            m_eventPosition = {event.mouse.x, event.mouse.y};
        }

        if(event.type == robot2D::Event::MouseReleased) {}

        if(event.type == robot2D::Event::MouseMoved) {}

        if(event.type == robot2D::Event::MouseWheel) {
            // no-op //
        }

        if(event.type == robot2D::Event::KeyPressed) {
            // no-op //
        }
    }

    std::uint32_t UISystem::addMousePressedCallback(MousePressedCallback&& callback) {
        m_mousePressedCallbacks.emplace_back(callback);
        return static_cast<std::uint32_t>(m_mousePressedCallbacks.size() - 1);
    }

    void UISystem::update(float dt) {
        for(auto& ent: m_entities) {
            bool contains = false;
            auto& hitbox = ent.getComponent<UIHitbox>();

            if((contains = hitbox.m_area.contains( { m_eventPosition.x, m_eventPosition.y } ))) {
                // movement next
            }

            if (contains) {
                for(auto it: m_mouseDownEvents) {
                    auto index = hitbox.callbackIDs[UIHitbox::CallbackID::MouseDown];
                    m_mousePressedCallbacks[index](ent, it);
                }
            }

        }

        m_mouseDownEvents.clear();
        m_eventPosition = {};
    }
}