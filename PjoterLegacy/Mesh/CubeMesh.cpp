#include "CubeMesh.h"

#include <iostream>

CubeMesh::CubeMesh()
{
	//std::cout << vertices.size() << std::endl;
	SetMesh(vertices, indices);
}
