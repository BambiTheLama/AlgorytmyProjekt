#pragma once

#include "../Scene/Mesh.h"

#include <vector>

/// <summary>
/// Klasa definiuj¹ca siatkê dla szeœcianu.
/// </summary>
class CubeMesh : public Mesh {
private:

    std::vector<float> vertices = {
        -1.0f, -1.0f,  1.0f,     0.0f, 0.0f, 1.0f,      1.0f, -1.0f,  1.0f,      0.0f, 0.0f, 1.0f,   	-1.0f,  1.0f,  1.0f,     0.0f, 0.0f, 1.0f,	 1.0f,  1.0f,  1.0f,     0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,     0.0f, 0.0f,-1.0f,     -1.0f,  1.0f, -1.0f,      0.0f, 0.0f,-1.0f,   	 1.0f, -1.0f, -1.0f,     0.0f, 0.0f,-1.0f,	 1.0f,  1.0f, -1.0f,     0.0f, 0.0f,-1.0f,
        -1.0f,  1.0f, -1.0f,     0.0f, 1.0f, 0.0f,     -1.0f,  1.0f,  1.0f,      0.0f, 1.0f, 0.0f,   	 1.0f,  1.0f, -1.0f,     0.0f, 1.0f, 0.0f,	 1.0f,  1.0f,  1.0f,     0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,     0.0f,-1.0f, 0.0f,      1.0f, -1.0f, -1.0f,      0.0f,-1.0f, 0.0f,   	-1.0f, -1.0f,  1.0f,     0.0f,-1.0f, 0.0f,	 1.0f, -1.0f,  1.0f,     0.0f,-1.0f, 0.0f,
         1.0f, -1.0f, -1.0f,     1.0f, 0.0f, 0.0f,      1.0f,  1.0f, -1.0f,      1.0f, 0.0f, 0.0f,   	 1.0f, -1.0f,  1.0f,     1.0f, 0.0f, 0.0f,	 1.0f,  1.0f,  1.0f,     1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,    -1.0f, 0.0f, 0.0f,     -1.0f, -1.0f,  1.0f,     -1.0f, 0.0f, 0.0f,   	-1.0f,  1.0f, -1.0f,    -1.0f, 0.0f, 0.0f,	-1.0f,  1.0f,  1.0f,    -1.0f, 0.0f, 0.0f,		
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,    // strona 1
        2, 1, 3,
        4, 5, 6,    // strona 2
        6, 5, 7,
        8, 9, 10,    // strona 3
        10, 9, 11,
        12, 13, 14,    // strona 4
        14, 13, 15,
        16, 17, 18,    // strona 5
        18, 17, 19,
        20, 21, 22,    // strona 6
        22, 21, 23,
    };

public:

    CubeMesh();

};