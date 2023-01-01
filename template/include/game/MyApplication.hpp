#pragma once

#include <robot2DGame/Application.hpp>

class MyApplication: public robot2D::GameApplication {
public:
    MyApplication();
    MyApplication(const MyApplication& application) = delete;
    MyApplication& operator=(const MyApplication& application) = delete;
    MyApplication(MyApplication&& application) = delete;
    MyApplication& operator=(MyApplication&& application) = delete;
    ~MyApplication() override = default;

    void setup() override;
protected:
    void handleEvents(const robot2D::Event& event) override;
    void update(float dt) override;
    void render() override;
private:
    void registerStates();
};
