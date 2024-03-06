#pragma once

#include "../Scene/Mesh.h"

#include <vector>

/// <summary>
/// Klasa definiuj¹ca siatkê dla p³aszczyzny.
/// </summary>
class PlaneMesh : public Mesh {
private:
	// Wsplrzedne wierzcholkow:
	std::vector<float> vertices = {
		-1.000000, 0.000000,  1.000000, 0.0000, 1.0000, 0.0000,
		 1.000000, 0.000000,  1.000000, 0.0000, 1.0000, 0.0000,
		-1.000000, 0.000000, -1.000000, 0.0000, 1.0000, 0.0000,
		 1.000000, 0.000000, -1.000000, 0.0000, 1.0000, 0.0000
	};
	// Indeksy scian:
	std::vector<unsigned int> indices = {
		0, 2, 1,
		2, 3, 1,
	};

	// Kolory (dla kazdego wierzcholka):
	float plane_cols[18] = {
		1.0000, 1.0000, 1.0000,
		1.0000, 1.0000, 1.0000,
		1.0000, 1.0000, 1.0000,
		1.0000, 1.0000, 1.0000,
		1.0000, 1.0000, 1.0000,
		1.0000, 1.0000, 1.0000
	};

	float plane_texc[12] = {
		1.000000, 0.000000,
		0.000000, 1.000000,
		0.000000, 0.000000,
		1.000000, 0.000000,
		1.000000, 1.000000,
		0.000000, 1.000000
	};

public:
	PlaneMesh();
};