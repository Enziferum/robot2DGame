#include <game/GameState.hpp>
#include <game/Messages.hpp>

namespace {
    bool isGame = false;
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


struct Quad: public robot2D::Drawable {
    robot2D::Color color;
    robot2D::vec2f position;
    robot2D::vec2f size;

    robot2D::FloatRect getAABB() const {
        return {position.x, position.y, size.x, size.y};
    }

    void draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const override {
        robot2D::Transform transform;
        transform.translate(position);
        transform.scale(size);

        states.transform *= transform;
        states.color = color;
        target.draw(states);
    }
};

class Editor: public robot2D::Drawable {
public:
    Editor() = default;
    ~Editor() = default;

    void handleEvents(const robot2D::Event& event);
    bool validJumpDist(robot2D::vec2f mousePoint);
    std::size_t& getLastSize() { return lastEditorBlocksSize; }
    const std::vector<Quad>& getBlocks() const { return editorBlocks; }
    void draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const override;
private:
    std::vector<Quad> editorBlocks;

    bool vertical = false;
    bool initMouseMoveQuad = false;
    bool createBlockMode = false;
    bool lastValidCanJump = true;

// todo calc //
    robot2D::vec2f maxCanJumpDist = {200.F, 200.F};
    robot2D::vec2f defaultMouseMoveQuadSize = {100.F, 10.F};
    std::size_t lastEditorBlocksSize = 0.F;

    Quad mouseMoveQuad{};
    Quad preAddQuad{};
};

void Editor::handleEvents(const robot2D::Event& event) {
    if(!initMouseMoveQuad) {
        mouseMoveQuad.color = robot2D::Color::White;
        mouseMoveQuad.color.alpha = 127.F;
        mouseMoveQuad.size = defaultMouseMoveQuadSize;
        initMouseMoveQuad = true;
    }

    if(event.type == robot2D::Event::MouseMoved) {
        robot2D::vec2f move = {event.move.x, event.move.y};

        if(createBlockMode) {

            if(vertical) {
                auto halfSize = std::abs(preAddQuad.position.y - move.y);
                preAddQuad.size.y = halfSize * 2;
            } else {
                auto halfSize = std::abs(preAddQuad.position.x - move.x);
                preAddQuad.size.x = halfSize * 2;
            }
            mouseMoveQuad.size = preAddQuad.size;
            mouseMoveQuad.color.alpha = 200.F;
        } else {
            mouseMoveQuad.position = move - mouseMoveQuad.size / 2.F;
            lastValidCanJump = validJumpDist(move);
            if(lastValidCanJump) {
                mouseMoveQuad.color = robot2D::Color::White;
            } else {
                mouseMoveQuad.color = robot2D::Color::Red;
            }

            mouseMoveQuad.color.alpha = 127.F;
        }
    }

    if(event.type == robot2D::Event::KeyPressed) {
        if(event.key.code == robot2D::Key::Q) {
            vertical = !vertical;
            std::swap(mouseMoveQuad.size.x, mouseMoveQuad.size.y);
        }
        if(event.key.code == robot2D::Key::ESCAPE && createBlockMode) {
            createBlockMode = false;
            if(vertical){
                auto copy = defaultMouseMoveQuadSize;
                std::swap(copy.x, copy.y);
                mouseMoveQuad.size = copy;
            }
            else
                mouseMoveQuad.size = defaultMouseMoveQuadSize;
        }
    }

    if(event.type == robot2D::Event::MousePressed) {
        if(createBlockMode) {
            Quad quad;
            if(lastValidCanJump) {
                quad.color = robot2D::Color::White;
            } else {
                quad.color = robot2D::Color::Red;
            }
            quad.size = preAddQuad.size;
            quad.position = preAddQuad.position;
            editorBlocks.emplace_back(std::move(quad));
            createBlockMode = false;
            preAddQuad = {};
        }
        else {
            createBlockMode = true;
            preAddQuad.position = robot2D::vec2i{event.mouse.x, event.mouse.y}.as<float>()
                                  - mouseMoveQuad.size / 2.F;
            preAddQuad.size = mouseMoveQuad.size;
        }
    }
}

bool Editor::validJumpDist(robot2D::vec2f mousePoint) {
    if(editorBlocks.empty())
        return true;

    for(auto& it: editorBlocks) {
        auto aabb = it.getAABB();
        aabb.lx -= maxCanJumpDist.x;
        aabb.width += maxCanJumpDist.x;
        aabb.ly -= maxCanJumpDist.y;
        aabb.height += maxCanJumpDist.y;

        if(aabb.contains(mousePoint))
            return true;
    }

    return false;
}

void Editor::draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const {
    for(auto& it: editorBlocks)
        target.draw(it);
    target.draw(mouseMoveQuad);
}



Editor editor{};


void GameState::handleEvents(const robot2D::Event& event) {
    if(event.type == robot2D::Event::KeyPressed
       && event.key.code == robot2D::Key::P) {
        isGame = !isGame;
        if(isGame) {
            const auto& editorBlocks = editor.getBlocks();
            auto& lastEditorBlocksSize = editor.getLastSize();

            std::size_t currSize = editorBlocks.size();
            for(std::size_t it = lastEditorBlocksSize; it < currSize; ++it) {
                auto msg = m_messageBus.postMessage<PlatformEvent>(MessageID::AddPlatform);
                msg -> size = editorBlocks[it].size;
                msg -> position = editorBlocks[it].position;
            }

            if(lastEditorBlocksSize == 0)
                lastEditorBlocksSize = currSize;
            else
                lastEditorBlocksSize = currSize - 1;
        }
    }

    if(isGame)
        m_inputParser.handleEvent(event);
    else
        editor.handleEvents(event);
}

void GameState::handleMessages(const robot2D::Message& message) {
    m_world.handleMessages(message);
    m_gameUI.handleMessages(message);
}

void GameState::update(float dt) {
    if(isGame)
        m_inputParser.update();
    m_world.update(dt);
    m_gameUI.update(dt);
}

void GameState::render() {
    m_window -> draw(m_world);
    if(!isGame)
        m_window -> draw(editor);
    m_window -> draw(m_gameUI);
}
