#pragma once

#include <robot2D/Core/Event.hpp>
#include <robot2D/Core/Message.hpp>
#include <robot2D/Util/ResourceHandler.hpp>

namespace robot2D {
    class IStateMachine;
    class State {
    public:
        State(IStateMachine& machine);
        virtual ~State() = 0;

        virtual void setup() = 0;
        virtual void destroy() = 0;
        virtual void handleEvents(const robot2D::Event&) = 0;
        virtual void handleMessages(const robot2D::Message&) = 0;
        virtual void update(float dt) = 0;
        virtual void render() = 0;

    protected:
        IStateMachine& m_machine;
    };
}