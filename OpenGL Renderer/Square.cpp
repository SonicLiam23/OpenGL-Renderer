#include "Square.h"
#include <glad/glad.h>

#include "Shader.h"

unsigned int Square::vao = 0;
unsigned int Square::vbo = 0;
unsigned int Square::ebo = 0;
int Square::verticesAmt = 0;
int Square::indicesAmt = 0;

Square::Square(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    InitBuffers();
    shader = new Shader(vertexShaderPath, fragmentShaderPath);
}

void Square::Bind()
{
    glBindVertexArray(vao);
}

void Square::InitBuffers()
{
    if (vao != 0) return; // Already initialized

    float vertices[] = {
        //x      y     z        r     g     b
        -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    verticesAmt = sizeof(vertices) / sizeof(float) / 3; // 3 floats per vertex (x, y, z)
    indicesAmt = sizeof(indices) / sizeof(unsigned int);


    // Generare Element Buffer Object (ebo) to store the indices of the vertices
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // generate vertex buffers. For my referece, the vbo variable is just an ID/index to the actual vbo
    glGenBuffers(1, &vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // generate a Vertex Array Object (vao) to store the state of the vertex attributes
    glGenVertexArrays(1, &vao);

    // 1. bind Vertex Array Object
    glBindVertexArray(vao);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // colour attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // unbind the vao and vbo (good practice, not strictly necessary)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Square::CleanupBuffers() 
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    vao = vbo = ebo = 0;
}

void Square::GetIndecesAndVertices(int* i, int* v)
{
    
    if (v != nullptr)
    {
        *v = verticesAmt;
    }
    if (i != nullptr)
    {
        *i = indicesAmt;
    }
}

Shader* Square::GetShader()
{
    return shader;
}
