#include "core/Application.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

Application::Application(const std::string& title, int width, int height)
    : m_Title(title), m_Width(width), m_Height(height) {
    Init();
}

Application::~Application() {
    Shutdown();
}

void Application::Init() {
    // Initialize with error handling
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        m_Running = false;
        return;
    }
    // Tell GLFW we want OpenGL 3.3 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on macOS
#endif
    // creates context
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        m_Running = false;
        return;
    }
    // set current context
    glfwMakeContextCurrent(m_Window);
    // v-sync
    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        m_Running = false;
        return;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";

    glViewport(0, 0, m_Width, m_Height);
}

void Application::Run() {
    float lastTime = (float)glfwGetTime();
    // Render Loop
    while(m_Running && !glfwWindowShouldClose(m_Window)) {
        // Calculate time between frames
        float now = (float)glfwGetTime();
        float deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();   
        Update(deltaTime); 
        glfwSwapBuffers(m_Window);
    }       
}

void Application::Update(float deltaTime) {
    // ------------------------------------
    // Future Game Logic Goes Here
    // ------------------------------------
    (void)deltaTime; // Cast void since unused so far
}

void Application::Render() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //sets clear color
    glClear(GL_COLOR_BUFFER_BIT); //clears screen to that color
    //Future Draw calls go here
}

void Application::Shutdown() {
    if (m_Window) glfwDestroyWindow(m_Window);
    glfwTerminate();
}
