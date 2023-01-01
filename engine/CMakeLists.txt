set(INCLROOT ${PROJECT_SOURCE_DIR}/engine/include/robot2DGame)

set(ENGINE_SRC
        #Core
        ${INCLROOT}/Application.hpp
        ${INCLROOT}/State.hpp
        ${INCLROOT}/StateMachine.hpp
        ${INCLROOT}/Font.hpp
        ${INCLROOT}/QuadBatchRender.hpp
        ${INCLROOT}/Text.hpp

        #components
        ${INCLROOT}/components/DrawableComponent.hpp
        ${INCLROOT}/components/TransformComponent.hpp
        ${INCLROOT}/components/UIAnimationComponent.hpp
        ${INCLROOT}/components/UIHitbox.hpp
        ${INCLROOT}/components/BroadphaseComponent.hpp

        #systems
        ${INCLROOT}/systems/RenderSystem.hpp
        ${INCLROOT}/systems/UISystem.hpp
        ${INCLROOT}/systems/UIAnimationSystem.hpp

        #experimental
        ${INCLROOT}/experimental/ALHelpers.hpp
        ${INCLROOT}/experimental/ALResource.hpp
        ${INCLROOT}/experimental/AudioData.hpp
        ${INCLROOT}/experimental/Music.hpp
        ${INCLROOT}/experimental/Sound.hpp
        ${INCLROOT}/experimental/SoundBuffer.hpp
        ${INCLROOT}/experimental/SoundSource.hpp
        ${INCLROOT}/experimental/SoundStream.hpp
        ${INCLROOT}/experimental/Audio.hpp

        ${INCLROOT}/Api.hpp

        PARENT_SCOPE
        )

set(SRCROOT ${PROJECT_SOURCE_DIR}/engine/source)

set(ENGINE_SRC ${ENGINE_SRC}
        #Core
        ${SRCROOT}/Application.cpp
        ${SRCROOT}/State.cpp
        ${SRCROOT}/StateMachine.cpp
        ${SRCROOT}/Font.cpp
        ${SRCROOT}/Text.cpp

        #components
        ${SRCROOT}/components/DrawableComponent.cpp
        ${SRCROOT}/components/TextComponent.cpp
        ${SRCROOT}/components/TransformComponent.cpp

        #systems
        ${SRCROOT}/systems/RenderSystem.cpp
        ${SRCROOT}/systems/UISystem.cpp
        ${SRCROOT}/systems/UIAnimationSystem.cpp
        ${SRCROOT}/systems/TextSystem.cpp

        #experimental
        ${SRCROOT}/audio/ALHelpers.cpp
        ${SRCROOT}/audio/IAudioReader.cpp
        ${SRCROOT}/audio/Music.cpp
        ${SRCROOT}/audio/Sound.cpp
        ${SRCROOT}/audio/SoundBuffer.cpp
        ${SRCROOT}/audio/SoundSource.cpp
        ${SRCROOT}/audio/SoundStream.cpp
        ${SRCROOT}/audio/WaveReader.cpp
        ${SRCROOT}/audio/AudioDevice.cpp
        ${SRCROOT}/audio/ALResource.cpp

        ${SRCROOT}/Api.cpp

        PARENT_SCOPE
        )