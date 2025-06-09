#pragma once
class Shader;
class Cube
{
public:
    Cube();

    void Bind();

    static void InitBuffers();

    static void CleanupBuffers();

    void GetIndecesAndVertices(int* i, int* v);

private:
    static unsigned int vao, vbo, ebo;
    static int verticesAmt, indicesAmt;
};

