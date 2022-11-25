#pragma once

#include <robot2DGame/Application.hpp>

class MyApplication: public robot2D::GameApplication {
public:
    MyApplication();
    ~MyApplication() override = default;

    void setup() override;

protected:
    void handleEvents(const robot2D::Event& event) override;
    void update(float dt) override;
    void render() override;
private:
    void registerStates();
};
