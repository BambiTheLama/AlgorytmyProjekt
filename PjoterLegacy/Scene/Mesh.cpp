#include "Mesh.h"

#include <algorithm>

Mesh::Mesh()
{
	layout.Push<float>(3);
	layout.Push<float>(3);
}

Mesh::~Mesh()
{

}

void Mesh::SetMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
	Clear();
	_vertices = vertices;
	_indices = indices;

	IndexCount = indices.size();

	_verticesArray = _vertices.data();
	_indicesArray = _indices.data();

	vb.Set(_verticesArray, _vertices.size() * sizeof(float));
	
	va.AddBuffer(vb, layout);

	ib.Set(_indicesArray, _indices.size());
}

void Mesh::UpdateVerticesMesh(const std::vector<float>& vertices)
{
	_vertices.clear();
	_vertices = vertices;

	_verticesArray = _vertices.data();

	vb.Set(_verticesArray, _vertices.size() * sizeof(float));

	va.AddBuffer(vb, layout);
}

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
	SetMesh(vertices, indices);
}

VertexArray& Mesh::GetVa()
{
	return va;
}

IndexBuffer& Mesh::GetIb()
{
	return ib;
}

void Mesh::Clear()
{
	_vertices.clear();
	_indices.clear();
}
