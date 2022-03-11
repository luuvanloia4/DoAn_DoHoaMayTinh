#include "Window.h"

void Window::Initialize(unsigned int width, unsigned int height, const std::string& title) {
    m_Width = width;
    m_Height = height;
    m_LastWidth = m_Width;
    m_LastHeight = m_Height;
    m_Title = title;
    m_IsFullScreen = false;

    m_GLFWHandler = glfwCreateWindow(m_Width, m_Height, title.c_str(), nullptr, nullptr);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void*)window;
    g_Window.m_LastWidth = g_Window.m_Width;
    g_Window.m_LastHeight = g_Window.m_Height;
    g_Window.m_Width = width;
    g_Window.m_Height = height;
    glViewport(0, 0, width, height);
}

void Window::SwapScreenSize() {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (m_IsFullScreen) {
        glfwSetWindowPos(g_Window.Pointer(), (mode->width - m_LastWidth) / 2, (mode->height - m_LastHeight) / 2);
        glfwSetWindowSize(g_Window.Pointer(), m_LastWidth, m_LastHeight);
        m_IsFullScreen = false;
    }
    else {
        //Full screen
        glfwSetWindowPos(g_Window.Pointer(), 0, 0);
        glfwSetWindowSize(g_Window.Pointer(), mode->width, mode->height);
        m_IsFullScreen = true;
    }
}