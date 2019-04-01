#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Texture.h"

#include <vector>

using namespace std;

// vertexy obsahuji informace o pozici vrcholu
// koordinatech textury
struct Vertex {
	glm::vec3 Position;
	glm::vec2 TexCoords;
};

class Mesh {
public:

	// VAO je public, abychom k nemu meli pristup z budov
	unsigned int VAO;
	vector<unsigned int> indices;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
	{
		this->vertices = vertices;
		this->indices = indices;

		// chybny pozustatek z drivejsi verze, vsiml jsem si ho
		// az po vytvoreni trajektorie kamery, po odstraneni by se zmenil
		// tvar mesta, tak ho tady nechavam, abych nemuse delat trajektorii znova :) 
		_random = rand() % NUMBER_OF_TEXTURES;
		
		setupMesh();
	}

	vector<Vertex> getVertices() {
		return this->vertices;
	}

	vector<unsigned int> getIndices() {
		return this->indices;
	}

private:
	unsigned int VBO, EBO;
	vector<Vertex> vertices;

	int _random;

	// inicializace VAO, VBO a EBO
	void setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};
#endif