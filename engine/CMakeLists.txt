set(INCLROOT ${PROJECT_SOURCE_DIR}/engine/include/robot2DGame)

set(ENGINE_SRC
        #Core
        ${INCLROOT}/Application.hpp
        ${INCLROOT}/State.hpp
        ${INCLROOT}/StateMachine.hpp

        PARENT_SCOPE
        )

set(SRCROOT ${PROJECT_SOURCE_DIR}/engine/source)

set(ENGINE_SRC ${ENGINE_SRC}
        #Core
        ${SRCROOT}/Application.cpp
        ${SRCROOT}/State.cpp
        ${SRCROOT}/StateMachine.cpp

        PARENT_SCOPE
        )