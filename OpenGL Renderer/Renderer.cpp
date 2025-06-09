#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Input.h"
#include "Shader.h"
#include "Cube.h"

#include <iostream>

// STATICS
GLFWwindow* Renderer::window = nullptr;
Shader* Renderer::shader = nullptr;
float Renderer::deltaTime = 0.0f;
bool Renderer::firstMouse = true;
float Renderer::yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float Renderer::pitch = 0.0f;
float Renderer::lastX = 800.0f / 2.0;
float Renderer::lastY = 600.0 / 2.0;
float Renderer::fov = 45.0f;
glm::vec3 Renderer::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

/// <summary>
/// move out eventually, currently handles the mouse position
/// </summary>
/// <param name="window"></param>
/// <param name="xpos"></param>
/// <param name="ypos"></param>
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

Renderer::Renderer(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    if (window == nullptr)
    {
        InitOpenGL();
        Input::Get();
        shader = new Shader(vertexShaderPath, fragmentShaderPath);
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
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

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
    // ==== CREATE SQUARE ==== //
    Cube IAmASquare;
    int numOfIndices, numOfVertices;
    IAmASquare.GetIndecesAndVertices(&numOfIndices, &numOfVertices);
    shader->Use();

    // ==== SETUP PERSPECTIVE ==== //
    glm::mat4 model = glm::mat4(1.0f); // initialize matrix to identity matrix first
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);
    
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader->SetMat4("projection", proj);

    // ==== SETUP CAMERA ==== //
    // values are going to be fixed at FIRST, as camera faces straight. 
    const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    // starting Pos and Target of the camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    
    // cameraFront is a static for the camera
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
    

    unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
    unsigned int viewLoc = glGetUniformLocation(shader->ID, "view");
    glm::vec3 cubePositions[] = 
    {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // for deltaTime calc
    float lastFrame = 0.0f;
    // SHOW WINDOWWWWWWW!
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input here
        Input::Get()->ProcessInput(window);
        const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraUp;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraUp;

        // ==== rendering commands here ==== //

        // clear colour buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ==== UPDATE CAMERA ==== //

        cameraDirection = glm::normalize(cameraDirection);
        cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        cameraUp = glm::cross(cameraDirection, cameraRight);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront,  cameraUp);

        



        // USe Shader
        shader->Use();

        //CURRENTLY ONLY DRAWS ONE SQUARE
        //DRAW OBJECTS BELOW
        //pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        IAmASquare.Bind();

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            if (i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
                angle = glfwGetTime() * 25.0f;
            if (i % 2 == 0)
                model = glm::translate(model, glm::vec3(0.0f, sin(glfwGetTime()) * 0.1, 0.0f));
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader->SetMat4("model", model);

            //glDrawArrays(GL_TRIANGLES, 0, numOfVertices);
            glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

float Renderer::DeltaTime()
{
    return deltaTime;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (Renderer::firstMouse)
    {
        Renderer::lastX = xpos;
        Renderer::lastY = ypos;
        Renderer::firstMouse = false;
    }

    float xoffset = xpos - Renderer::lastX;
    float yoffset = Renderer::lastY - ypos;
    Renderer::lastX = xpos;
    Renderer::lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Renderer::yaw += xoffset;
    Renderer::pitch += yoffset;

    if (Renderer::pitch > 89.0f)
        Renderer::pitch = 89.0f;
    if (Renderer::pitch < -89.0f)
        Renderer::pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(Renderer::yaw)) * cos(glm::radians(Renderer::pitch));
    direction.y = sin(glm::radians(Renderer::pitch));
    direction.z = sin(glm::radians(Renderer::yaw)) * cos(glm::radians(Renderer::pitch));
    Renderer::cameraFront = glm::normalize(direction);
}
