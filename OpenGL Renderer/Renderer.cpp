#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Input.h"
#include "Shader.h"
#include "Square.h"

#include <iostream>

// STATICS
GLFWwindow* Renderer::window = nullptr;
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

Renderer::Renderer()
{
    if (window == nullptr)
    {
        InitOpenGL();
        Input::Get();
    }
}

int Renderer::InitOpenGL()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window and OpenGL context
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Print OpenGL version as a sanity check
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    return 0;
}

GLFWwindow* Renderer::GetWindow()
{
    return window;
}

void Renderer::Start()
{
    Square IAmASquare("shader.vs", "shader.fs");
    int numOfIndices, numOfVertices;
    IAmASquare.GetIndecesAndVertices(&numOfIndices, &numOfVertices);

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::scale(trans, glm::vec3(1.0f));
    // SHOW WINDOWWWWWWW!
    while (!glfwWindowShouldClose(window))
    {
        // input here
        Input::Get()->ProcessInput(window);

        // rendering commands here

        // clear colour buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // CURRENTLY ONLY DRAWS ONE SQUARE
        // DRAW OBJECTS BELOW
        // updates colour
        // shader.setFloat("someUniform", 1.0f);
        trans = glm::rotate(trans, glm::radians(1.0f), glm::vec3(0.0, 0.0, 1.0));

        IAmASquare.GetShader()->Use();

        unsigned int transformLoc = glGetUniformLocation(IAmASquare.GetShader()->ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        IAmASquare.Bind();
        glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0); // draw from 
        //glDrawArrays(GL_TRIANGLES, 0, numOfVertices); // draws from vertices array

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


