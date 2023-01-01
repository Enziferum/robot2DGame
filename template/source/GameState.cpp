#include <game/GameState.hpp>
#include <game/Messages.hpp>
#include <robot2DGame/Text.hpp>

#include <fstream>
#include <filesystem>
#include <array>
#include <unordered_set>

namespace {
    std::string mapPath = "res/map/sample.map";
}


bool saveMap(const std::string& path, const std::vector<Quad>& quads) {
    std::ofstream file{path, std::ios::binary};
    if(!file.is_open() || quads.empty())
        return false;

    auto bufferSize = quads.size() * sizeof(float) * 4;
    char* buffer = new char[bufferSize];

    char* bufferIt = buffer;
    for(const auto& it: quads) {
        memcpy(bufferIt, &it.position.x, sizeof(float));
        bufferIt += sizeof(float);
        memcpy(bufferIt, &it.position.y, sizeof(float));
        bufferIt += sizeof(float);
        memcpy(bufferIt, &it.size.x, sizeof(float));
        bufferIt += sizeof(float);
        memcpy(bufferIt, &it.size.y, sizeof(float));
        bufferIt += sizeof(float);
    }

    file.write(buffer, bufferSize);
    file.close();

    delete[] buffer;

    return true;
}

bool loadMap(const std::string& path, std::vector<Quad>& quads) {
    std::ifstream file{path, std::ios::binary};
    if(!file.is_open())
        return false;

    auto bufferSize = std::filesystem::file_size(path);
    if(bufferSize == 0 )
        return false;

    char* buffer = new char[bufferSize];
    file.read(buffer, bufferSize);
    file.close();

    auto offset = 16;
    auto elemCount = bufferSize / offset;
    char* bufferIt = buffer;
    for(auto it = 0; it < elemCount; ++it) {
        Quad quad;
        memcpy(&quad.position.x, bufferIt , sizeof(float));
        memcpy(&quad.position.y, bufferIt  + 4, sizeof(float));
        memcpy(&quad.size.x, bufferIt  + 8, sizeof(float));
        memcpy(&quad.size.y, bufferIt  + 12, sizeof(float));
        bufferIt  += offset;
        quads.emplace_back(std::move(quad));
    }
    delete[] buffer;

    return true;
}

void postAddBlockEvent(robot2D::MessageBus& messageBus, const Quad& quad) {
    auto msg = messageBus.postMessage<PlatformEvent>(MessageID::AddPlatform);
    msg -> size = quad.size;
    msg -> position = quad.position;
}



GameState::GameState(robot2D::IStateMachine& machine,
                     robot2D::AppContext& context, robot2D::MessageBus& messageBus):
            robot2D::State{machine, context, messageBus},
            m_world{messageBus},
            m_gameUI{messageBus}
            {}

void GameState::setup() {
    /// Initalize Resources
    m_world.setup(m_inputParser);
    m_gameUI.setup();
}

void GameState::destroy() {
    /// Destroy Resources
}

template<bool val>
struct BoolConstant {
    using Type = BoolConstant<val>;
    static constexpr bool value = val;
};

using true_type = BoolConstant<true>;
using false_type = BoolConstant<false>;

template<typename FROM, typename TO>
struct IsConvertableHelper {
private:
    static void aux(TO);

    template<typename T, typename, typename = decltype(aux(std::declval<T>()))>
    static true_type test(void*);

    template<typename>
    static false_type test(...);
public:
    using Type = decltype(test<FROM>(nullptr));
};

template<typename FROM, typename TO>
struct IsConvertableT: IsConvertableHelper<FROM, TO>::Type {};

template<typename FROM, typename TO>
using is_convertible_t = typename IsConvertableT<FROM, TO>::Type;

template<typename FROM, typename TO>
constexpr bool is_convertible_v = IsConvertableT<FROM, TO>::value;

template<typename KeyActionType,
        typename Container = std::unordered_map<KeyActionType, robot2D::Key>,
        typename = std::enable_if_t<std::is_enum_v<KeyActionType>>>
struct EditorBinding {
    using pairType = typename Container::value_type;

    EditorBinding(std::initializer_list<pairType> list) {
        m_keyMap = std::unordered_map<KeyActionType, robot2D::Key>{list};
    }

    bool add(KeyActionType actionType, robot2D::Key key) {
        if(m_keyMap.find(actionType))
            return false;
        return m_keyMap.insert(std::make_pair<KeyActionType, robot2D::Key>(actionType, key)).inserted;
    }

    const robot2D::Key& getKey(const KeyActionType& keyActionType) const {
        return m_keyMap.at(keyActionType);
    }

    bool remove(KeyActionType actionType) {
        return static_cast<bool>(m_keyMap.erase(actionType));
    }
private:
    Container m_keyMap;
};

void GameState::handleEvents(const robot2D::Event& event) {

    enum class ActionType {
        Editor, SaveMap, LoadMap
    };

    EditorBinding<ActionType> m_editorBinding{{ActionType::Editor, robot2D::Key::E},
                                              {ActionType::SaveMap, robot2D::Key::I},
                                              {ActionType::LoadMap, robot2D::Key::O}};

    if(event.type == robot2D::Event::KeyPressed
       && event.key.code == m_editorBinding.getKey(ActionType::Editor)) {
        if(m_stateType == StateType::Editor)
            m_stateType = StateType::Game;
        else
            m_stateType = StateType::Editor;

        if(m_stateType == StateType::Game) {
            const auto& editorBlocks = m_editor.getBlocks();
            auto& lastEditorBlocksSize = m_editor.getLastSize();

            std::size_t currSize = editorBlocks.size();
            for(std::size_t it = lastEditorBlocksSize; it < currSize; ++it) {
                postAddBlockEvent(m_messageBus, editorBlocks[it]);
            }

            if(lastEditorBlocksSize == 0)
                lastEditorBlocksSize = currSize;
            else
                lastEditorBlocksSize = currSize - 1;
        }
    }

    if(event.type == robot2D::Event::KeyPressed) {
        if(event.key.code == m_editorBinding.getKey(ActionType::SaveMap))
            saveMap(mapPath, m_editor.getBlocks());
        if(event.key.code == m_editorBinding.getKey(ActionType::LoadMap)) {
            std::vector<Quad> blocks;
            if(loadMap(mapPath, blocks))
                m_editor.setBlocks(blocks);
        }
    }

    if(m_stateType == StateType::Game)
        m_inputParser.handleEvent(event);
    else
        m_editor.handleEvents(event);
}

void GameState::handleMessages(const robot2D::Message& message) {
    m_world.handleMessages(message);
    m_gameUI.handleMessages(message);
}

void GameState::update(float dt) {
    if(m_stateType == StateType::Game)
        m_inputParser.update();
    m_world.update(dt);
    m_gameUI.update(dt);
}

void GameState::render() {
    m_window -> draw(m_world);
    if(m_stateType == StateType::Editor)
        m_window -> draw(m_editor);
    m_window -> draw(m_gameUI);
}
