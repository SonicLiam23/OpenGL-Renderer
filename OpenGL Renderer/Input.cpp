#include "Input.h"
// MOSTLY UNUSED FOR NOW 
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Input* Input::instance = nullptr;

Input::Input()
{

}

void Input::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

Input* Input::Get()
{
    if (instance != nullptr)
    {
        instance = new Input();
    }
    return instance;
}

