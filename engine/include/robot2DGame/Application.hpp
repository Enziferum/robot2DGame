#pragma once

#include <robot2D/Application.hpp>
#include <robot2D/Core/MessageBus.hpp>
#include "StateMachine.hpp"

namespace robot2D {
    class GameApplication: public Application {
    public:
        GameApplication();
        virtual ~GameApplication() = 0;

        virtual void setup();
    protected:
        virtual void handleEvents(const robot2D::Event& event) = 0;
        void handleMessages();
        virtual void update(float dt) = 0;
        virtual void render() = 0;

    protected:
        robot2D::MessageBus m_bus;
        StateMachine m_machine;
        AppContext m_context;
    };
}