#include "Skybox.h"

Skybox::Skybox() {

	fPerlinNoise2D = new float[moon_d * moon_d];
	perlinNoise();

	texture = generateSkyboxTexture();
	textureBlackWhite = generateSkyboxTextureBlackWhite();
	setupSkybox();
}

// vykresleni skyboxu, musime nastavit spravnou texturu
void Skybox::Draw(bool volumetricLight) 
{
	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	if (volumetricLight) glBindTexture(GL_TEXTURE_CUBE_MAP, textureBlackWhite);
	else glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

// vytvoreni skyboxu
void Skybox::setupSkybox()
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
}

// generovani textur pro jednotlive strany
unsigned int Skybox::generateSkyboxTexture() 
{
	int position, x, y;
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	float red=12, green=36, blue=60;
	std::vector<Color> data_side(width * height);
	std::fill(data_side.begin(), data_side.end(), Color());
	for (int i = width - 1; i >= 0; i--) {
		for (int j = height - 1; j >= 0; j--) {
			position = i * width + j;
			if (i > height / 2) {
				int position = i * width + j;
				data_side[position].r = 24;
				data_side[position].g = 24;
				data_side[position].b = 24;
			}
			else if (rand() % 7000 < 2) {
				data_side[position].r = 255;
				data_side[position].g = 255;
				data_side[position].b = 255;
			}
			else {
				data_side[position].r = red;
				data_side[position].g = green;
				data_side[position].b = blue;
			}
		}
		red -= (float) 12 / height;
		green -= (float) 36 / height;
		blue -= (float) 60 / height;
		if (red < 0) red = 0;
		if (green < 0) red = 0;
		if (blue < 0) red = 0;
	}

	std::vector<Color> data_side1(width * height);
	std::vector<Color> data_side2(width * height);
	std::vector<Color> data_side3(width * height);

	data_side1 = data_side;
	data_side2 = data_side;
	data_side3 = data_side;

	std::vector<GLubyte> data_bottom(width * height * 3);
	std::fill(data_bottom.begin(), data_bottom.end(), 24);
	
	std::vector<GLubyte> data_top(width * height * 3);
	std::fill(data_top.begin(), data_top.end(), 0);
	for (int i = width - 1; i >= 0; i--) {
		for (int j = height - 1; j >= 0; j--) {
			position = (i * width * 3) + (j * 3);
			if (rand() % 7000 < 2) {
				data_top.at(position) = 255;
				data_top.at(position + 1) = 255;
				data_top.at(position + 2) = 255;
			}
			else {
				data_top.at(position) = 0;
				data_top.at(position + 1) = 0;
				data_top.at(position + 2) = 0;
			}
		}
	}

	std::vector<Color> data_moon(width * height);
	data_moon = data_side;
	int moon_r = moon_d / 2;
	int moon_r2 = moon_r * 2;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int length = sqrt(pow(moon_x - i, 2) + pow(moon_y - j, 2));
			int random = rand() % 700 + 450;
			if (length <= moon_r2 && length > moon_r && random > length) {
				int position = i * width + j;
				if (rand() % 7000 < 2) {
					data_moon[position].r = 255;
					data_moon[position].g = 255;
					data_moon[position].b = 255;
				}
				else {
					float add = (127 - ((127.0f / moon_d) * length)) * 0.5f;
					data_moon[position].r = data_moon[position].r + (int)add;
					data_moon[position].g = data_moon[position].g + (int)add;
					data_moon[position].b = data_moon[position].b + (int)add;
				}
			}
		}
	}

	int i2 = 0, j2 = 0;
	bool isIn = false;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (sqrt(pow(moon_x - i, 2) + pow(moon_y - j, 2)) <= moon_r) {
				int position = i * width + j;				
				data_moon[position].r = 255 - fPerlinNoise2D[i2 * moon_d + j2] * (255 - pow((float)60 / 255, 2.2f) * 255);
				data_moon[position].g = 255 - fPerlinNoise2D[i2 * moon_d + j2] * (255 - pow((float)90 / 255, 2.2f) * 255);
				data_moon[position].b = 255 - fPerlinNoise2D[i2 * moon_d + j2] * (255 - pow((float)150 / 255, 2.2f) * 255);
				isIn = true;
			}	
			if (j >= moon_x - moon_r) j2++;
		}
		if (isIn) i2++;
		isIn = false;
		j2 = 0;
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (i > height / 2) {
				int position = i * width + j;
				data_moon[position].r = 24;
				data_moon[position].g = 24;
				data_moon[position].b = 24;
			}
		}
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data_moon[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data_side1[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data_top[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data_bottom[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data_side2[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data_side3[0]);


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

// generovani textur pro jednotlive strany pro occlusion mapu
unsigned int Skybox::generateSkyboxTextureBlackWhite() {

	int position, x, y;
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	std::vector<GLubyte> data_side(width * height * 3);
	std::fill(data_side.begin(), data_side.end(), 0);

	std::vector<GLubyte> data_moon(width * height * 3);
	data_moon = data_side;
	int moon_r = moon_d / 2;
	int moon_r2 = moon_r * 2;

	int i2 = 0, j2 = 0;
	bool isIn = false;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (sqrt(pow(moon_x - i, 2) + pow(moon_y - j, 2)) <= moon_r / 2) {
				int position = (i * width + j) * 3;
				float color = 255 - ((sqrt(pow(moon_x - i, 2) + pow(moon_y - j, 2)) / (moon_r / 2)) * 255);
				if ((sqrt(pow(moon_x - i, 2) + pow(moon_y - j, 2)) / (moon_r / 2)) < 51) color = 200;
				data_moon[position] = color;
				data_moon[position + 1] = color;
				data_moon[position + 2] = color;
				isIn = true;
			}
			if (j >= moon_x - moon_r) j2++;
		}
		if (isIn) i2++;
		isIn = false;
		j2 = 0;
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (i > height / 2) {
				int position = (i * width + j) * 3;
				data_moon[position] = 0;
				data_moon[position + 1] = 0;
				data_moon[position + 2] = 0;
			}
		}
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data_moon[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data_side[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data_side[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data_side[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data_side[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data_side[0]);


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

// aproximace Perlinova sumu
void Skybox::perlinNoise() {

	float *fNoiseSeed2D = new float[moon_d * moon_d];
	int *perlinNoise = new int[moon_d * moon_d];
	for (int i = 0; i < moon_d * moon_d; i++) {
		fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;
	}

	for (int x = 0; x < moon_d; x++)
		for (int y = 0; y < moon_d; y++)
		{
			float fNoise = 0.0f;
			float fScaleAcc = 0.0f;
			float fScale = 1.0f;

			for (int o = 0; o < 8; o++)
			{
				int nPitch = moon_d >> o;
				int nSampleX1 = (x / nPitch) * nPitch;
				int nSampleY1 = (y / nPitch) * nPitch;

				int nSampleX2 = (nSampleX1 + nPitch) % moon_d;
				int nSampleY2 = (nSampleY1 + nPitch) % moon_d;

				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
				float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

				float fSampleT = (1.0f - fBlendX) * fNoiseSeed2D[nSampleY1 * moon_d + nSampleX1] + fBlendX * fNoiseSeed2D[nSampleY1 * moon_d + nSampleX2];
				float fSampleB = (1.0f - fBlendX) * fNoiseSeed2D[nSampleY2 * moon_d + nSampleX1] + fBlendX * fNoiseSeed2D[nSampleY2 * moon_d + nSampleX2];

				fScaleAcc += fScale;
				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
				fScale = fScale / 1.35f;
			}

			fPerlinNoise2D[y * moon_d + x] = fNoise / fScaleAcc;
			if (fPerlinNoise2D[y * moon_d + x] >= 0.39f) fPerlinNoise2D[y * moon_d + x] -= 0.39f;
		}
}