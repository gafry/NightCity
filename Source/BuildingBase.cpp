#include "BuildingBase.h"

// konstruktor vybere nahodnou texturu
// pro budovu a taky nahodnou barvu budovy
BuildingBase::BuildingBase()
{
	_random = rand() % NUMBER_OF_TEXTURES;

	int random = rand() % 15;

	switch (random)
	{
	case 0:
	case 1:
	case 2:
	case 6:
	case 8:
		_color = glm::vec3(0.67f, 0.92f, 0.94f);
		break;
	case 3:
	case 4:
	case 5:
	case 9:
		_color = glm::vec3(1.0f, 1.0f, 1.0f);
		break;
	case 7:
		_color = glm::vec3(0.71f, 0.72f, 0.96f);
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		_color = glm::vec3(0.88f, 1.00f, 0.57f);
		break;
	default:
		break;
	}
}

// tato funkce vytvori Mesh plaste kvadru, neboli stran budovy
// uklada vertexy s informacemi o koordinatech textur
// a pozici vertexu
void BuildingBase::processMesh(float min_height, float x, float z)
{
	float width = _width;
	float height = _height;
	float depth = _depth;

	vector<Vertex> vertices;
	Vertex vertex;

	int number_of_windows_per_width = 15;

	double width_index = ((double)((int)width / number_of_windows_per_width) / NUMBER_OF_WINDOWS_WIDTH);
	double depth_index = ((double)((int)depth / number_of_windows_per_width) / NUMBER_OF_WINDOWS_WIDTH);
	double height_index = ((double)((int)(height - min_height) / number_of_windows_per_width) / NUMBER_OF_WINDOWS_HEIGHT);

	// A
	vertex.Position = glm::vec3(x, min_height, z);
	vertex.TexCoords = glm::vec2(0.0f, height_index);
	vertices.push_back(vertex);

	// B
	vertex.Position = glm::vec3(x + width, min_height, z);
	vertex.TexCoords = glm::vec2(width_index, height_index);
	vertices.push_back(vertex);

	// C
	vertex.Position = glm::vec3(x, height, z);
	vertex.TexCoords = glm::vec2(0.0f, 0.0f);
	vertices.push_back(vertex);

	// D
	vertex.Position = glm::vec3(x + width, height, z);
	vertex.TexCoords = glm::vec2(width_index, 0.0f);
	vertices.push_back(vertex);

	// E
	vertex.Position = glm::vec3(x, min_height, z - depth);
	vertex.TexCoords = glm::vec2(width_index + width_index + depth_index, height_index);
	vertices.push_back(vertex);

	// F
	vertex.Position = glm::vec3(x + width, min_height, z - depth);
	vertex.TexCoords = glm::vec2(width_index + depth_index, height_index);
	vertices.push_back(vertex);

	// G
	vertex.Position = glm::vec3(x, height, z - depth);
	vertex.TexCoords = glm::vec2(width_index + width_index + depth_index, 0.0f);
	vertices.push_back(vertex);

	// H
	vertex.Position = glm::vec3(x + width, height, z - depth);
	vertex.TexCoords = glm::vec2(width_index + depth_index, 0.0f);
	vertices.push_back(vertex);

	// A
	vertex.Position = glm::vec3(x, min_height, z);
	vertex.TexCoords = glm::vec2(width_index + width_index + depth_index + depth_index, height_index);
	vertices.push_back(vertex);

	// C
	vertex.Position = glm::vec3(x, height, z);
	vertex.TexCoords = glm::vec2(width_index + width_index + depth_index + depth_index, 0.0f);
	vertices.push_back(vertex);

	unsigned int indicies_arr[] = {
		0, 3, 1,
		3, 0, 2,
		1, 7, 5,
		7, 1, 3,
		5, 6, 4,
		6, 5, 7,
		4, 9, 8,
		9, 4, 6
	};

	vector<unsigned int> indices(indicies_arr, indicies_arr + 24);

	_meshes.push_back(Mesh(vertices, indices));
}

// tato funkce vytvori mesh pro strechu budovy,
// opet uklada vertexy s informacemi o koordinatech textury
// a jejich pozici
void BuildingBase::processRoof(float x, float z) {

	float width = _width;
	float height = _height;
	float depth = _depth;

	vector<Vertex> vertices;
	Vertex vertex;

	// A
	vertex.Position = glm::vec3(x, height, z);
	vertex.TexCoords = glm::vec2(0.0f, 0.0f);
	vertices.push_back(vertex);

	// B
	vertex.Position = glm::vec3(x + width, height, z);
	vertex.TexCoords = glm::vec2(1.0f, 0.0f);
	vertices.push_back(vertex);

	// C
	vertex.Position = glm::vec3(x, height, z - depth);
	vertex.TexCoords = glm::vec2(0.0f, 1.0f);
	vertices.push_back(vertex);

	// D
	vertex.Position = glm::vec3(x + width, height, z - depth);
	vertex.TexCoords = glm::vec2(1.0f, 1.0f);
	vertices.push_back(vertex);

	unsigned int indicies_arr[] = {
		0, 3, 1,
		3, 0, 2,
	};

	vector<unsigned int> indices(indicies_arr, indicies_arr + 6);

	_meshes.push_back(Mesh(vertices, indices));
}