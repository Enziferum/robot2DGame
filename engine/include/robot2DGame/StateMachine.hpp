#pragma once

#include <unordered_map>
#include <functional>
#include <vector>
#include <memory>
#include <cassert>

#include <robot2D/Core/Event.hpp>

#include "State.hpp"
#include "AppContext.hpp"

namespace robot2D {

    class IStateMachine {
    public:
        IStateMachine() = default;
        virtual ~IStateMachine() = 0;

        virtual void pushState(const StateID& id) = 0;
        virtual void pop() = 0;
        virtual void clear() = 0;
    };

    class StateMachine: public IStateMachine {
    public:
        StateMachine(robot2D::MessageBus& bus);
        ~StateMachine() = default;

        void setup(AppContext& m_context);
        void destroy();

        template<typename T, typename ...Args>
        void registerState(const StateID& id, Args&&...args);

        void pushState(const StateID& id) override;
        void pop() override;
        void clear() override;

        void handleEvent(const Event& event);
        void handleMessages(const Message& message);
        void update(float dt);
        void render();
    private:
        friend class GameApplication;
        void setWindow(RenderWindow* window);
        void processChanges();
        std::unique_ptr<State> createState(const StateID& id);

        enum class Action {
            push,
            pop,
            clear
        };

        struct PendingChange {
            StateID id;
            Action m_action;
            PendingChange(const Action& action): m_action(action){}
        };

        robot2D::MessageBus& m_bus;
        std::vector<std::unique_ptr<State> > m_stack;
        std::vector<PendingChange> m_processChanges;
        std::unordered_map<StateID, std::function<std::unique_ptr<State>() > > m_factories;
        robot2D::RenderWindow* m_window;
        AppContext* m_context;
    };


    template<typename T, typename... Args>
    void StateMachine::registerState(const StateID& id, Args &&... args) {
        m_factories[id] = [this, &args...]() {
            return std::make_unique<T>(*this, *m_context, m_bus, std::forward<Args>(args)...);
        };
    }
}