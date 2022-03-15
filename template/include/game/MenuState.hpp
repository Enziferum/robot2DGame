#pragma once

#include <robot2DGame/State.hpp>

class MenuState final: public robot2D::State {
public:
    MenuState();
    ~MenuState() override = default;

    void setup() override;
    void destroy() override;

    void handleEvents(const robot2D::Event& ) override;
    void handleMessages(const robot2D::Message&) override;
    void update(float dt) override;
    void render() override;

private:
};