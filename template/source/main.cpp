#include <robot2D/Engine.hpp>
#include <game/MyApplication.hpp>

#include <game/DummyEditor.hpp>
#include <random>

#include "IniReader.hpp"

int runTemplate() {
    robot2D::EngineConfiguration engineConfiguration;
    engineConfiguration.windowSize = {800, 600};
    engineConfiguration.windowTitle = "Template Game";
    engineConfiguration.windowContext.renderDimensionType = robot2D::WindowContext::RenderDimensionType::TwoD;
    ROBOT2D_RUN_ENGINE(MyApplication, engineConfiguration)
}

namespace robot2D {

}


class Camera2D {
public:
    Camera2D() = default;
    ~Camera2D() = default;

    void resetView(const robot2D::vec2f windowSize) {
        m_view.reset({0, 0, windowSize.x, windowSize.y});
        m_mousePos = {windowSize / 2.F};
    }

    void handleEvents(const robot2D::Event& event) {}

    void update(float dt, robot2D::RenderWindow* window) {
        (void)dt;

        constexpr float deltaOffset = 0.5F;

        auto mousePos = window -> getMousePos();
        robot2D::vec2f delta = (mousePos - m_mousePos) * deltaOffset;
        m_mousePos = mousePos;

        if(window -> isMousePressed(robot2D::Mouse::MouseMiddle)) {
            auto center = m_view.getCenter();
            center.x += delta.x * 2.F;
            center.y += delta.y * 2.F;
            m_view.setCenter(center);
        }

    }

    const robot2D::View& getCameraView() const {
        return m_view;
    }
private:
    robot2D::View m_view{};
    robot2D::vec2f m_mousePos{};
};


class ExampleApplication: public robot2D::GameApplication {
public:
    ExampleApplication() = default;
    ~ExampleApplication() override = default;

    void setup() override;
protected:
    void handleEvents(const robot2D::Event& event) override;
    void update(float dt) override;
    void render() override;

    void setupQuads();
private:
    Camera2D m_camera2D;
    std::vector<Quad> m_quads;

    robot2D::Text m_text;
    robot2D::Font m_font;

    Quad player;
};

void ExampleApplication::setup() {
    auto sz = m_window -> getSize();
    m_camera2D.resetView(sz.as<float>());

    m_font.loadFromFile("res/font/game_font.ttf");
    m_text.setPosition({200, 200});
    m_text.setText("Robot2D Engine!");
    m_text.setFont(m_font);
    m_text.setColor(robot2D::Color::White);
    auto textSize = m_font.calculateSize("Robot2D Engine!");
    setupQuads();
}

void ExampleApplication::setupQuads() {
    std::random_device dev;
    std::mt19937 rng(dev());

    constexpr int quadsCount = 2000;

    auto randomColor = [&]() {
        std::uniform_int_distribution<std::mt19937::result_type> red(0, 255);
        std::uniform_int_distribution<std::mt19937::result_type> green(0, 255);
        std::uniform_int_distribution<std::mt19937::result_type> blue(0, 255);

        return robot2D::Color{ red(rng), green(rng), blue(rng)};
    };

    auto randSize = [&]() {
        std::uniform_int_distribution<std::mt19937::result_type> x(10, 150);
        std::uniform_int_distribution<std::mt19937::result_type> y(10, 150);
        return robot2D::vec2f { x(rng), y(rng)};
    };

    auto randPosition = [&]() {
        std::uniform_int_distribution<std::mt19937::result_type> x(-10000, 10000);
        std::uniform_int_distribution<std::mt19937::result_type> y(-10000, 10000);
        return robot2D::vec2f { x(rng), y(rng)};
    };

    for(int i = 0; i < quadsCount; ++i) {
        Quad q;
        q.position = randPosition();
        q.size = randSize();
        q.color = randomColor();
        m_quads.emplace_back(std::move(q));
    }

}

void ExampleApplication::handleEvents(const robot2D::Event& event) {

    m_camera2D.handleEvents(event);
}

void ExampleApplication::update(float dt) {

    m_camera2D.update(dt, m_window);
}

void ExampleApplication::render() {
    m_window -> clear();
    m_window -> beforeRender();
    m_window -> setView(m_camera2D.getCameraView());

    for(const auto& quad: m_quads)
        m_window -> draw(quad);

    m_window -> draw(player);
    m_window -> draw(m_text);

    m_window -> afterRender();
    m_window -> display();
}


template<>
robot2D::vec2f mobb::Value::as() const {
    auto [x, y] = divide(m_value, 'x');
    if(x.empty() && y.empty())
        return {};
    return {std::stof(x), std::stof(y)};
}

int writeConfig() {
    mobb::IniReader reader;
    mobb::Section windowSection{"Window"};
  //  windowSection["size"] = mobb::Value{ to_string(robot2D::vec2f{1280, 920}) };
    reader[windowSection.getName()] = windowSection;

    if(!reader.saveToFile("res/default.ini")) {
        return -1;
    }

    return 0;
}

int runQuadTree() {
    mobb::IniReader iniReader{};
    if(!iniReader.loadFromFile("res/config.ini")){
        return -1;
    }

    if(!iniReader.hasSection("Window"))
        return -2;

    if(!iniReader["Window"].hasValue("size"))
        return -3;

    auto windowSize = iniReader["Window"]["size"].as<robot2D::vec2f>();

    robot2D::EngineConfiguration engineConfiguration;
    engineConfiguration.windowSize = windowSize.as<unsigned int>();
    engineConfiguration.windowTitle = "QuadTree";
    engineConfiguration.windowContext.renderDimensionType = robot2D::WindowContext::RenderDimensionType::TwoD;
    ROBOT2D_RUN_ENGINE(ExampleApplication, engineConfiguration)
}


int main() {
   return runQuadTree();
}