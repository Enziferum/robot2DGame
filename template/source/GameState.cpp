#include <game/GameState.hpp>
#include <game/Messages.hpp>
#include <robot2DGame/Text.hpp>

#include <fstream>
#include <filesystem>
#include <array>

namespace {
    bool isGame = false;
    std::string mapPath = "res/map/sample.map";
}

struct Quad: public robot2D::Drawable {
    robot2D::Color color;
    robot2D::vec2f position;
    robot2D::vec2f size;
    robot2D::Texture* texture;
    robot2D::FloatRect aabb;
    bool updatedAABB = false;
    robot2D::FloatRect& getAABB() {
        if(aabb == robot2D::FloatRect{})
            aabb = {position.x, position.y, size.x, size.y};
        return aabb;
    }

    void draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const override {
        robot2D::Transform transform;
        transform.translate(position);
        transform.scale(size);

        states.transform *= transform;
        states.color = color;
        states.texture = texture;
        target.draw(states);
    }
};

struct DebugCollider: public robot2D::Drawable{
    float borderSize = 1.F;
    robot2D::FloatRect aabb;
    robot2D::Color borderColor = robot2D::Color::Green;
    robot2D::Texture* texture;

    void draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const override {
        std::array<robot2D::Transform, 4> quads;
        quads[0].translate(aabb.lx, aabb.ly);
        quads[0].scale(aabb.width,borderSize);

        quads[1].translate(aabb.lx, aabb.ly);
        quads[1].scale(borderSize, aabb.height);

        quads[2].translate(aabb.lx, aabb.ly + aabb.height);
        quads[2].scale(aabb.width,borderSize);

        quads[3].translate(aabb.lx + aabb.width, aabb.ly);
        quads[3].scale(borderSize,aabb.height);

        for(auto& it: quads) {
            states.transform = it;
            states.color = borderColor;
            states.texture = texture;
            target.draw(states);
        }
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
    void setBlocks(std::vector<Quad> blocks) {editorBlocks = blocks;}
    void draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const override;
private:
    std::vector<Quad> editorBlocks;

    bool vertical = false;
    bool initMouseMoveQuad = false;
    bool createBlockMode = false;
    bool lastValidCanJump = true;

// todo calc //
    robot2D::vec2f maxCanJumpDist = {120.F, 90.F};
    robot2D::vec2f defaultMouseMoveQuadSize = {100.F, 10.F};
    std::size_t lastEditorBlocksSize = 0.F;

    Quad mouseMoveQuad{};
    Quad preAddQuad{};

    robot2D::Texture m_texture;
    robot2D::Font m_font;
    robot2D::Text m_mouseText;

    bool drawJumpZones = true;
};

void Editor::handleEvents(const robot2D::Event& event) {

    if(!initMouseMoveQuad) {
        mouseMoveQuad.color = robot2D::Color::White;
        mouseMoveQuad.color.alpha = 127.F;
        mouseMoveQuad.size = defaultMouseMoveQuadSize;

        initMouseMoveQuad = true;
        m_texture.loadFromFile("res/textures/block.png");
        mouseMoveQuad.texture = &m_texture;
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
        if(event.key.code == robot2D::Key::H)
            drawJumpZones = !drawJumpZones;
    }

    if(event.type == robot2D::Event::MousePressed) {
        if(createBlockMode) {
            Quad quad;
            quad.color = robot2D::Color::White;
            quad.size = preAddQuad.size;
            quad.position = preAddQuad.position;
            quad.texture = &m_texture;
            editorBlocks.emplace_back(std::move(quad));
            createBlockMode = false;
            preAddQuad = {};

            if(vertical){
                auto copy = defaultMouseMoveQuadSize;
                std::swap(copy.x, copy.y);
                mouseMoveQuad.size = copy;
            }
            else
                mouseMoveQuad.size = defaultMouseMoveQuadSize;
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
        if(!it.updatedAABB) {
            aabb.lx -= maxCanJumpDist.x;
            aabb.width += maxCanJumpDist.x * 2.F;
            aabb.ly -= maxCanJumpDist.y;
            aabb.height += maxCanJumpDist.y * 2.F;
            it.aabb = aabb;
            it.updatedAABB = true;
        }

        if(aabb.contains(mousePoint))
            return true;
    }

    return false;
}

void Editor::draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const {
    for(auto& it: editorBlocks) {
        target.draw(it);
        if(drawJumpZones) {
            DebugCollider collider;
            collider.aabb = it.aabb;
            collider.texture = it.texture;
            target.draw(collider);
        }

    }
    target.draw(mouseMoveQuad);
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

Editor editor{};

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

void GameState::handleEvents(const robot2D::Event& event) {
    if(event.type == robot2D::Event::KeyPressed
       && event.key.code == robot2D::Key::P) {
        isGame = !isGame;
        if(isGame) {
            const auto& editorBlocks = editor.getBlocks();
            auto& lastEditorBlocksSize = editor.getLastSize();

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
        if(event.key.code == robot2D::Key::W)
            saveMap(mapPath, editor.getBlocks());
        if(event.key.code == robot2D::Key::O) {
            std::vector<Quad> blocks;
            if(loadMap(mapPath, blocks))
                editor.setBlocks(blocks);
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
