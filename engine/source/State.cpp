#include <robot2DGame/State.hpp>
#include <robot2DGame/StateMachine.hpp>

namespace robot2D {
    State::State(robot2D::IStateMachine& stateMachine,
                 robot2D::AppContext& context,
                 robot2D::MessageBus& messageBus):
                 m_machine{stateMachine},
                 m_context{context},
                 m_messageBus{messageBus},
                 m_window(nullptr) {
                    m_window = &m_context.get<RenderWindow>(AppContextID::Window);
                 }

    State::~State() {}

    void State::handleMessages(const robot2D::Message& message) {
        /// Default no - op
    }

    bool State::isInitialized() {
        return m_initalized;
    }

}