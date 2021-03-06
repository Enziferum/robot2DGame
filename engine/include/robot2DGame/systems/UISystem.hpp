#pragma once

#include <functional>
#include <robot2D/Ecs/System.hpp>
#include <robot2D/Core/Event.hpp>

namespace robot2D {

    using MousePressedCallback = std::function<void(robot2D::ecs::Entity, std::uint64_t)>;

    class UISystem: public robot2D::ecs::System {
    public:
        enum Flags
        {
            RightMouse = 0x1,
            LeftMouse = 0x2,
            MiddleMouse = 0x4,
        };


        UISystem(robot2D::MessageBus&);
        ~UISystem() override = default;

        void handleEvents(const robot2D::Event& event);
        void update(float dt) override;

        std::uint32_t addMousePressedCallback(MousePressedCallback&& callback);
    private:
        std::vector<MousePressedCallback> m_mousePressedCallbacks;
        std::vector<Flags> m_mouseDownEvents;
        robot2D::vec2i m_eventPosition{};
    };

}