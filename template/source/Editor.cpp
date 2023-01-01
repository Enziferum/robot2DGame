#include <game/DummyEditor.hpp>

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
        robot2D::vec2f move = { event.move.x, event.move.y};

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
    (void)states;

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