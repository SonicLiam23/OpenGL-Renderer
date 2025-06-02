#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// a double pointer bc i want to edit the ORIGINAL pointer... weird but okay lol
int InitOpenGL(GLFWwindow*& window);
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	GLFWwindow* window = nullptr;
	if (InitOpenGL(window) != 0)
	{
		return -1; // Initialization failed
	}
    Shader shader("shader.vs", "shader.fs"); // you can name your shader files however you like


    float vertices[] = {
        // Position       // R     G     B
        0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top right
	   -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // top left
		0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,// bottom right
	   -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f// bottom left
	};

    unsigned int indices[] = {
        0, 1, 2, 
        1, 3, 2,  
    };

 //   float vertices[] = {
 //       // first triangle
 //       -0.9f, -0.5f, 0.0f,  // left 
 //        0.0f, -0.5f, 0.0f,  // right
 //       -0.45f, 0.5f, 0.0f,  // top 
 //       // second triangle
 //      //0.0f, -0.5f, 0.0f,  // left, SAME AS RIGHT ON T1
 //        0.9f, -0.5f, 0.0f,  // right
 //        0.45f, 0.5f, 0.0f,   // top 
 //   };

	//unsigned int indices[] = {
	//	0, 1, 2, // first triangle
	//	1, 3, 4, // second triangle 
	//};


	// Generare Element Buffer Object (EBO) to store the indices of the vertices
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // generate vertex buffers. For my referece, the VBO variable is just an ID/index to the actual VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// generate a Vertex Array Object (VAO) to store the state of the vertex attributes
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // colour attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // unbind the VAO and VBO (good practice, not strictly necessary)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); 
    // the above could be honestly classed as an eldritch horror

	int numOfVertices = sizeof(vertices) / sizeof(float) / 3; // 3 floats per vertex (x, y, z)
    int numOfIndices = sizeof(indices) / sizeof(unsigned int);

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::scale(trans, glm::vec3(1.0f));
    

    // SHOW WINDOWWWWWWW!
    while (!glfwWindowShouldClose(window))
    {
        // input here
        processInput(window);

        // rendering commands here

        // clear colour buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

        // DRAW OBJECTS BELOW
        // updates colour
        // shader.setFloat("someUniform", 1.0f);
        trans = glm::rotate(trans, glm::radians(1.0f), glm::vec3(0.0, 0.0, 1.0));
        
        shader.Use();

        unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0); // draw from 
        //glDrawArrays(GL_TRIANGLES, 0, numOfVertices); // draws from vertices array

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

int InitOpenGL(GLFWwindow*& window)
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

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}