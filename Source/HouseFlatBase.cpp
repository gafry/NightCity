#include "HouseFlatBase.h"

HouseFlatBase::HouseFlatBase() 
{

}

// pridani pozice, kde se bude nachazet instance
// teto budovy
void HouseFlatBase::addPosition(float x, float z)
{
	_positions.push_back(glm::vec3(x, 0.0f, z));
}

// vrati pocet instanci teto budovy
int HouseFlatBase::getPositionsSize()
{
	return _positions.size();
}

// zpracuje VAO budovy. Vytvorime si buffer,
// do ktereho nacteme transformacni matice pro kazdou
// instanci teto budovy
// pote pridame do VAO kazdeho meshe (jsou jen dva - strecha
// a plast) atributy pro transformacni matice
void HouseFlatBase::processVAO()
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

// pri vykreslovani nastavime barvu budovy, pak pro kazdy mesh
// nabindujeme texturu a vykreslime instance budovy pomoci instancingu
void HouseFlatBase::draw(Shader shader)
{
	Texture * _texture = Texture::getInstance();

	shader.setVec3("color", _color);
	for (unsigned int i = 0; i < _meshes.size(); i++)
	{
		if (i == _meshes.size() - 1) _texture->bindRoofTexture(_random % NUMBER_OF_ROOF_TEXTURES);
		else _texture->bindTexture(_random);
		glBindVertexArray(_meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, _meshes[i].indices.size(), GL_UNSIGNED_INT, 0, _positions.size());
		glBindVertexArray(0);
	}
}