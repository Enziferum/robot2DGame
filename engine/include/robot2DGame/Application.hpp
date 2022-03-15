#pragma once

#include <robot2D/Graphics/RenderWindow.hpp>
#include <robot2D/Core/MessageBus.hpp>

namespace robot2D {
    class Application {
    public:
        Application();
        virtual ~Application() = 0;

        virtual void setup() = 0;
        virtual void registerStates() = 0;

        void run();
    private:
        void handleEvents();
        void handleMessages();
        void update(float dt);
        void render();

        void destroy();
    private:
        robot2D::RenderWindow m_window;
        robot2D::MessageBus m_bus;
    };
}