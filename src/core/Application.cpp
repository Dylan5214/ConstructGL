#include "core/Application.h"
#include "renderer/Shader.h"
#include "../external/stb_image.h"

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


    // Triangle setup

    float vertices[] = {
        //pos               //colors
        0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //Shader
    m_Shader = new Shader("../assets/shaders/shader.vs", "../assets/shaders/shader.fs");
    m_Shader->use();
    m_Shader->setInt("ourTexture", 0);

    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../assets/textures/container.jpg", &width, &height, &nrChannels, 0);
    std::cout << "Image loaded: " << width << "x" << height << " channels: " << nrChannels << "\n";

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture\n";
    }
    stbi_image_free(data);
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
        Render();
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
    m_Shader->use();
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Application::Shutdown() {
    if (m_Window) glfwDestroyWindow(m_Window);
    delete m_Shader;
    glfwTerminate();
}
