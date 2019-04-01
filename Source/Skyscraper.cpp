#include "Skyscraper.h"

Skyscraper::Skyscraper(float x, float z, float width, float depth)
{
	_x = x;
	_z = z;
	_width_base = width;
	_depth_base = depth;
	_height_base = rand() % 10000 + 6000;
	if (_height_base > 15000) _height_base += rand() % 5000;
	_height_base /= 10;
	_model = glm::translate(_model, glm::vec3(_x, 0.0f, _z));

	_random2 = rand() % (NUMBER_OF_TEXTURES - 4) + 4;
	_random3 = rand() % (NUMBER_OF_TEXTURES - 4) + 4;
	_random4 = rand() % (NUMBER_OF_TEXTURES - 4) + 4;
	_random5 = rand() % (NUMBER_OF_TEXTURES - 4) + 4;

	createSkyscraper();
}

// vytvoreni mrakodrapu z nekolika mensich
// kvadru (budov)
void Skyscraper::createSkyscraper()
{
	if (rand() % 15 == 0) {
		_width = _width_base;
		_depth = _depth_base;
		_height = _height_base;
		processMesh();
		processRoof();

		return;
	}

	// spodek je v maximalnim tvaru
	int random_h = rand() % (int)((_height_base / 3) * 2);
	_width = _width_base;
	_depth = _depth_base;
	_height = random_h;
	processMesh();
	processRoof();

	_width = (_width_base * 2) / 3, _height = _height_base, _depth = (_depth_base * 2) / 3;
	int x, z, half_x = _width_base / 2, half_z = _depth_base / 2;

	int random = rand() % 3 + 2;

	// pomocne vektory se souradnicemi, pokud se
	// nove vznikla mala cast blizi nejake souradnici,
	// zahodi se (kvuli snizeni z-fightingu)
	vector<float> xs;
	vector<float> zs;
	vector<float> xwidths;
	vector<float> zdepths;

	for (int i = 0; i < random; i++) {

		if (_width > half_x) x = rand() % (int)(_width_base - _width);
		else x = rand() % (int)_width + (half_x - _width);

		if (_depth > half_z) z = - rand() % (int)(_depth_base - _depth);
		else z = - rand() % (int)_depth - (half_z - _depth);

		// snizeni z-fightingu
		if (_height > random_h) {
			bool isOk = true;
			for (int i = 0; i < xs.size(); i++) {
				if (x - xs[i] >= -12 && x - xs[i] <= 12) isOk = false;
			}
			for (int i = 0; i < xwidths.size(); i++) {
				if ((x + _width) - xwidths[i] >= -12 && (x + _width) - xwidths[i] <= 12) isOk = false;
			}
			for (int i = 0; i < zs.size(); i++) {
				if (z - zs[i] >= -12 && z - zs[i] <= 12) isOk = false;
			}
			for (int i = 0; i < zdepths.size(); i++) {
				if ((z - _depth) - zdepths[i] >= -12 && (z - _depth) - zdepths[i] <= 12) isOk = false;
			}
			if (isOk)
			{
				xs.push_back(x);
				zs.push_back(z);
				xwidths.push_back(x + _width);
				zdepths.push_back(z - _depth);

				processMesh(random_h, x, z);
				processRoof(x, z);
			}
			// chybny pozustatek z drivejsi verze v tride Mesh, vsiml jsem si ho
			// az po vytvoreni trajektorie kamery, po odstraneni by se zmenil
			// tvar mesta, tak tady musim volat dvakrat rand(), ktere by se volaly v Meshi, 
			// abych nemuse delat trajektorii znova :) 
			else
			{
				rand() % NUMBER_OF_TEXTURES;
				rand() % NUMBER_OF_TEXTURES;
			}
		}

		_width = rand() % (int)(_width_base - 5) + 5;
		_height = _height - rand() % (int)(_height / 3);
		_depth = rand() % (int)(_depth_base - 5) + 5;
	}
}

// mrakodrapy se vykresluji bez instancingu, kazdy je unikatni,
// musime proto nastavit model transformaèní matici,
// nasledne vykreslujeme jednotlive meshe
void Skyscraper::draw(Shader shader)
{
	Texture * _texture = Texture::getInstance();
	shader.setVec3("color", _color);
	shader.setMat4("model", _model);
	if (_meshes.size() > 2)
	for (unsigned int i = 0; i < _meshes.size(); i++)
	{
		// strecha nebo bok
		if (i % 2 == 1) _texture->bindRoofTexture(_random % NUMBER_OF_ROOF_TEXTURES);
		else if (i == 0) _texture->bindTexture(_random);
		else if (i == 2) _texture->bindTexture(_random2);
		else if (i == 4) _texture->bindTexture(_random3);
		else if (i == 6) _texture->bindTexture(_random4);
		else _texture->bindTexture(_random5);
		glBindVertexArray(_meshes[i].VAO);
		glDrawElements(GL_TRIANGLES, _meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}