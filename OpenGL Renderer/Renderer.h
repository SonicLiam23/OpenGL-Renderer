#pragma once
class GLFWwindow;

class Renderer
{
public:
    Renderer();
    int InitOpenGL();

    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 800;
    GLFWwindow* GetWindow();

    void Start();
    
private:
    static GLFWwindow* window;

};

