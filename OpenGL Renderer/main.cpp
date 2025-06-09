
#include "Renderer.h"

int main()
{
    Renderer renderer("shader.vs", "shader.fs");


    renderer.Start();

    return 0;
}
