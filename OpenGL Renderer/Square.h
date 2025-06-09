#pragma once
class Shader;
class Square
{
public:
    Square(const char* vertexShaderPath, const char* fragmentShaderPath);

    void Bind();

    static void InitBuffers();

    static void CleanupBuffers();

    void GetIndecesAndVertices(int* i, int* v);

    Shader* GetShader();

private:
    static unsigned int vao, vbo, ebo;
    static int verticesAmt, indicesAmt;
    Shader* shader;
};

