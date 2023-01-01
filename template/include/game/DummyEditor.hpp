#pragma once
#include <vector>
#include <array>

#include <robot2D/Graphics/Drawable.hpp>
#include <robot2D/Graphics/RenderTarget.hpp>
#include <robot2D/Core/Event.hpp>
#include <robot2DGame/Text.hpp>

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

    void draw(robot2D::RenderTarget& target,
              robot2D::RenderStates states) const override {
        robot2D::Transform transform;
        transform.translate(position);
        transform.scale(size);

        states.transform *= transform;
        states.color = color;
        target.draw(states);
    }
};

struct DebugCollider: public robot2D::Drawable {
    float borderSize = 1.F;
    robot2D::FloatRect aabb;
    robot2D::Color borderColor = robot2D::Color::Green;
    robot2D::Texture* texture;

    void draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const override {
        std::array<robot2D::Transform, 4> quads;
        quads[0].translate(aabb.lx, aabb.ly);
        quads[0].scale(aabb.width, borderSize);

        quads[1].translate(aabb.lx, aabb.ly);
        quads[1].scale(borderSize, aabb.height);

        quads[2].translate(aabb.lx, aabb.ly + aabb.height);
        quads[2].scale(aabb.width,borderSize);

        quads[3].translate(aabb.lx + aabb.width, aabb.ly);
        quads[3].scale(borderSize, aabb.height);

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