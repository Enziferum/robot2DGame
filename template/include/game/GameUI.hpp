#pragma once
#include <robot2D/Core/Message.hpp>
#include <robot2D/Graphics/Drawable.hpp>

class GameUI final: public robot2D::Drawable {
public:
    GameUI();
    ~GameUI() = default;

    void setup();

    void handleMessages(const robot2D::Message& message);
    void update(float dt);
    void draw(robot2D::RenderTarget& target, robot2D::RenderStates states) const override;
private:

};