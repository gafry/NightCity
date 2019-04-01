#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <math.h>

#include "Shader.h"

struct Color {
	GLubyte r;
	GLubyte g;
	GLubyte b;
};

class Skybox
{

public:

	Skybox();
	void Draw(bool volumetricLight = false);

private:

	unsigned int VBO;
	unsigned int texture;
	unsigned int textureBlackWhite;
	unsigned int VAO;

	float skyboxVertices;

	float *fPerlinNoise2D;
	int width = 2048, height = 2048;
	int moon_x = 1024;
	int moon_y = 1024;
	int moon_d = 400;

	void setupSkybox();

	unsigned int generateSkyboxTexture();
	unsigned int generateSkyboxTextureBlackWhite();
	void perlinNoise();
};

#endif