#include <robot2DGame/StateMachine.hpp>

namespace robot2D {

    IStateMachine::~IStateMachine() {}

    StateMachine::StateMachine(robot2D::MessageBus& bus):
            m_bus{bus},
            m_stack(),
            m_processChanges(),
            m_factories{},
            m_window{nullptr}{}


    void StateMachine::setWindow(RenderWindow* window) {
        if(window == nullptr)
            exit(1);
        m_window = window;
    }

    void StateMachine::setup(AppContext& context) {
        m_context = &context;
        if(!m_context -> store<RenderWindow>(AppContextID::Window, m_window))
            exit(2);
    }

    void StateMachine::pushState(const StateID &id) {
        auto change = PendingChange(Action::push);
        change.id = id;
        m_processChanges.emplace_back(change);
    }

    void StateMachine::pop() {
        m_processChanges.emplace_back(PendingChange(Action::pop));
    }

    void StateMachine::clear() {
        m_processChanges.emplace_back(PendingChange(Action::clear));
    }

    void StateMachine::handleEvent(const Event &event) {
        for(auto it = m_stack.rbegin(); it != m_stack.rend(); ++it) {
            it->get()->handleEvents(event);
        }
        processChanges();
    }

    void StateMachine::handleMessages(const Message& message) {
        for(auto it = m_stack.rbegin(); it != m_stack.rend(); ++it) {
            it-> get() -> handleMessages(message);
        }
    }

    void StateMachine::update(float dt) {
        for(auto it = m_stack.rbegin(); it != m_stack.rend(); ++it) {
            it -> get() -> update(dt);
        }
        processChanges();
    }

    void StateMachine::render() {
        for(auto it = m_stack.rbegin(); it != m_stack.rend(); ++it) {
            it->get()->render();
        }
    }

    void StateMachine::processChanges() {
        for(auto& it: m_processChanges) {
            switch(it.m_action) {
                case Action::push: {
                    auto state = createState(it.id);
                    if(!state -> isInitialized())
                        state -> setup();
                    m_stack.emplace_back(std::move(state));
                    break;
                }
                case Action::pop:
                    m_stack.pop_back();
                    break;
                case Action::clear:
                    m_stack.clear();
                    break;
            }
        }
        m_processChanges.clear();
    }

    std::unique_ptr<State> StateMachine::createState(const StateID& id) {
        auto found = m_factories.find(id);
        assert(found != m_factories.end());
        return found->second();
    }

    void StateMachine::destroy() {
        for(auto it = m_stack.rbegin(); it != m_stack.rend(); ++it) {
            it -> get() -> destroy();
        }
    }


}