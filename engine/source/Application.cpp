#include <robot2DGame/Application.hpp>

namespace robot2D {
    GameApplication::GameApplication():
            Application(),
            m_bus{},
            m_machine{m_bus} {}

    GameApplication::~GameApplication() {}

    void GameApplication::handleMessages() {
        /// Default No - op
    }

    void GameApplication::setup() {
        m_machine.setWindow(m_window);
        m_machine.setup(m_context);
    }
}