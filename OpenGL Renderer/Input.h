#pragma once
// MOSTLY UNUSED FOR NOW!
class GLFWwindow;

class Input
{
private:
    Input();
    static Input* instance;

public:
    static Input* Get();
    void ProcessInput(GLFWwindow* window);

};
