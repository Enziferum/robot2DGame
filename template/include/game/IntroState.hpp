#pragma once

#include <robot2DGame/State.hpp>

class IntroState final: public robot2D::State {
public:
    IntroState();
    ~IntroState() override = default;

    void setup() override;
    void destroy() override;

    void handleEvents(const robot2D::Event&) override;
    void handleMessages(const robot2D::Message&) override;

    void update(float dt) override;
    void render() override;
private:
};