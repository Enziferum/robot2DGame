#pragma once

#include <robot2DGame/Application.hpp>

class MyApplication: public robot2D::Application {
public:
    MyApplication();
    ~MyApplication() override = default;

    void setup() override;
    void registerStates() override;
};
