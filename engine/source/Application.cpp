#include <robot2DGame/Application.hpp>

namespace robot2D {
    GameApplication::GameApplication():
            Application(),
            m_bus{},
            m_machine{m_bus} {}

    GameApplication::~GameApplication() {}

    void GameApplication::handleMessages() {
        robot2D::Message message{};
        while (m_bus.pollMessages(message))
            m_machine.handleMessages(message);
    }

    void GameApplication::setup() {
        m_machine.setWindow(m_window);
        m_machine.setup(m_context);
    }
}