#pragma once

#include <robot2D/Graphics/RenderWindow.hpp>
#include <robot2D/Core/MessageBus.hpp>
#include <robot2D/Util/ResourceHandler.hpp>
#include "AppContext.hpp"

namespace robot2D {
    using StateID = uint32_t;
    class IStateMachine;

    class State {
    public:
        State(IStateMachine& machine, AppContext& context, TMessageBus<>& messageBus);
        virtual ~State() = 0;

        virtual void setup() = 0;
        virtual void destroy() {};

        virtual void handleEvents(const Event& event) = 0;
        virtual void handleMessages(const Message& message);
        virtual void update(float dt) = 0;
        virtual void render() = 0;

        bool isInitialized();
    protected:
        IStateMachine& m_machine;
        AppContext& m_context;
        TMessageBus<>& m_messageBus;
        RenderWindow* m_window;
        bool m_initalized = false;
    };
}