#pragma once

namespace robot2D {
    using StateID = uint32_t;

    class IStateMachine {
    public:
        IStateMachine() = default;
        virtual ~IStateMachine() = 0;

        virtual void pushState(const StateID& id) = 0;
        virtual void pop() = 0;
        virtual void clear() = 0;
    };

}