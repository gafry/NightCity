#include "Road.h"

Road::Road(float width, float depth, short type) 
{
	_width = width;
	_depth = depth;
	_type = type;

	processRoad();
}

// pridani pozice, kde se bude nachazet instance
// teto silnice
void Road::addPosition(float x, float z)
{
	_positions.push_back(glm::vec3(x, 0.0f, z));
}

// zpracuje VAO budovy. Vytvorime si buffer,
// do ktereho nacteme transformacni matice pro kazdou
// instanci teto budovy
// pote pridame do VAO kazdeho meshe atributy pro transformacni matice
void Road::processVAO()
{
	glm::mat4 * matrices = new glm::mat4[_positions.size()];
	for (int n = 0; n < _positions.size(); n++) {
		glm::mat4 model;
		model = glm::translate(model, _positions[n]);
		matrices[n] = model;
	}

	glGenBuffers(1, &_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _buffer);
	glBufferData(GL_ARRAY_BUFFER, _positions.size() * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < _meshes.size(); i++)
	{
		unsigned int VAO = _meshes[i].VAO;
		glBindVertexArray(VAO);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);

		glBindVertexArray(0);
	}
}

// nabindujeme texturu a vykreslime instance budovy pomoci instancingu
void Road::draw()
{
	Texture * _texture = Texture::getInstance();

	glBindBuffer(GL_ARRAY_BUFFER, _buffer);
	_texture->bindRoadTexture();
	for (unsigned int i = 0; i < _meshes.size(); i++)
	{
		glBindVertexArray(_meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, _meshes[i].indices.size(), GL_UNSIGNED_INT, 0, _positions.size());
		glBindVertexArray(0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// tato funkce vytvori mesh pro silnici
// uklada vertexy s informacemi o koordinatech textury
// a pozici vertexu
void Road::processRoad() {

	float width = _width;
	float depth = _depth;
	short type = _type;

	vector<Vertex> vertices;
	Vertex vertex;
	glm::vec2 dl, dr, ul, ur;

	if (type == ROAD_VERTICAL) {
		dl = glm::vec2(0.0f, 0.0f);
		dr = glm::vec2(1.0f, 0.0f);
		ul = glm::vec2(0.0f, 1.0f);
		ur = glm::vec2(1.0f, 1.0f);
	}
	else {
		dl = glm::vec2(1.0f, 0.0f);
		dr = glm::vec2(1.0f, 1.0f);
		ul = glm::vec2(0.0f, 0.0f);
		ur = glm::vec2(0.0f, 1.0f);
	}

	// A
	vertex.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	vertex.TexCoords = dl;
	vertices.push_back(vertex);

	// B
	vertex.Position = glm::vec3(width, 0.0f, 0.0f);
	vertex.TexCoords = dr;
	vertices.push_back(vertex);

	// C
	vertex.Position = glm::vec3(0.0f, 0.0f, - depth);
	vertex.TexCoords = ul;
	vertices.push_back(vertex);

	// D
	vertex.Position = glm::vec3(width, 0.0f, -depth);
	vertex.TexCoords = ur;
	vertices.push_back(vertex);

	unsigned int indicies_arr[] = {
		0, 1, 2,
		1, 2, 3,
	};

	vector<unsigned int> indices(indicies_arr, indicies_arr + 6);

	_meshes.push_back(Mesh(vertices, indices));
}