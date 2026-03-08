#pragma once
#include <string>

struct GLFWwindow;

class Application {
public:
    Application(const std::string& title, int width, int height);
    ~Application();

    void Run();

private:
    void Init();
    void Update(float deltaTime);
    void Render();
    void Shutdown();

    GLFWwindow* m_Window = nullptr;
    std::string m_Title;
    int m_Width, m_Height;
    bool m_Running = true;
};