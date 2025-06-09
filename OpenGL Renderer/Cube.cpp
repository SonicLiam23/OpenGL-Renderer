#include "Cube.h"
#include <glad/glad.h>

#include "Shader.h"

unsigned int Cube::vao = 0;
unsigned int Cube::vbo = 0;
unsigned int Cube::ebo = 0;
int Cube::verticesAmt = 0;
int Cube::indicesAmt = 0;

Cube::Cube()
{
    InitBuffers();
}

void Cube::Bind()
{
    glBindVertexArray(vao);
}

void Cube::InitBuffers()
{
    if (vao != 0) return; // Already initialized

    float vertices[] = 
    {
      0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f, // +++ 0
     -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 1.0f, // -++ 1
     -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f, // --+ 2
     -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f, // --- 3
      0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f, // +-- 4
      0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f, // ++- 5
      0.5f, -0.5f, 0.5f,     1.0f, 0.0f, 1.0f, // +-+ 6
     -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f  // -+- 7
    };

    unsigned int indices[] = 
    {
        3, 4, 5,
        5, 7, 3,

        2, 6, 0, 
        0, 1, 2, 

        1, 7, 3,
        3, 2, 1,

        0, 5, 4, //
        4, 6, 0,

        3, 4, 6,
        6, 2, 3,

        7, 5, 0,
        0, 1, 7
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

void Cube::CleanupBuffers() 
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    vao = vbo = ebo = 0;
}

void Cube::GetIndecesAndVertices(int* i, int* v)
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
