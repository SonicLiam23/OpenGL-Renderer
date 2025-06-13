#pragma once
class GLFWwindow;
class Shader;
#include <glm/glm.hpp>

class Renderer
{
public:
    Renderer(const char* vertexShaderPath, const char* fragmentShaderPath);
    int InitOpenGL();

    const unsigned int SCR_WIDTH = 1920;
    const unsigned int SCR_HEIGHT = 786;
    GLFWwindow* GetWindow();

    void Start();
    static float DeltaTime();

    static bool firstMouse;
    static float yaw;	
    static float pitch;
    static float lastX;
    static float lastY;
    static float fov;
    static glm::vec3 cameraFront;
    
private:
    static GLFWwindow* window;
    static Shader* shader;
    static float deltaTime;
    

};

