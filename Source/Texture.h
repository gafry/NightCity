#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <glad/glad.h>

#define WINDOW_SIZE 90
#define WINDOW_WIDTH 150
#define WINDOW_HEIGHT 80
#define NUMBER_OF_WINDOWS_WIDTH 20
#define NUMBER_OF_WINDOWS_HEIGHT 40
#define WINDOW_AREA (WINDOW_WIDTH * WINDOW_HEIGHT)
#define FRAME 20
#define ROAD_WIDTH 120
#define NUMBER_OF_TEXTURES 30
#define NUMBER_OF_ROOF_TEXTURES 6

class Texture {

public:

	static Texture * _instance;

	// navrhovy vzor Singleton
	static Texture * getInstance() {
		if (_instance == nullptr) _instance = new Texture();
		return _instance;
	}

	// bind textur
	void bindTexture(int number) 
	{		
		glBindTexture(GL_TEXTURE_2D, textures[number]);
	}

	void bindRoofTexture(int number)
	{
		glBindTexture(GL_TEXTURE_2D, roofTextures[number]);
	}

	void bindRoadTexture() 
	{
		glBindTexture(GL_TEXTURE_2D, roadTextures[0]);
	}

private:

	GLuint * textures;
	GLuint * roofTextures;
	GLuint * roadTextures;
	
	// volani jednotlivych funkci, ktere vytvareji textury
	Texture() {

		textures = new GLuint[NUMBER_OF_TEXTURES];
		roofTextures = new GLuint[NUMBER_OF_ROOF_TEXTURES];
		roadTextures = new GLuint[1];
		glGenTextures(NUMBER_OF_TEXTURES, textures);
		glGenTextures(NUMBER_OF_ROOF_TEXTURES, roofTextures);
		glGenTextures(1, roadTextures);

		createGrayRoof();
		createColoredRoof();
		createRoad();

		for (int c = 0; c < NUMBER_OF_TEXTURES; c++) {
			createRandomWindow(c);
		}		
	}

	void createGrayRoof() 
	{
		std::vector<GLubyte> data;
		unsigned int width = 1;
		unsigned int height = 1;
		for (int i = 0; i < height * width * 3; i++) {
			data.push_back(20);
		}

		setTexture(roofTextures[0], data, 1, 1);

		std::vector<GLubyte> data2;
		for (int i = 0; i < height * width * 3; i++) {
			data2.push_back(25);
		}

		setTexture(roofTextures[1], data2, 1, 1);

		std::vector<GLubyte> data3;
		for (int i = 0; i < height * width * 3; i++) {
			data3.push_back(30);
		}

		setTexture(roofTextures[2], data3, 1, 1);
	}

	void createColoredRoof()
	{
		unsigned int width = 40;
		unsigned int height = 40;
		unsigned int position;
		std::vector<GLubyte> data(width * height * 3);
		std::fill(data.begin(), data.end(), 30);

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				position = (i * width + j) * 3;
				if ((i < 5 || i >= 35 || j < 5 || j >= 35) ||
					(i >= 10 && i < 15 && j >= 10 && j < 30) ||
					(i >= 25 && i < 30 && j >= 10 && j < 30) ||
					(j >= 10 && j < 15 && i >= 10 && i < 30) ||
					(j >= 25 && j < 30 && i >= 10 && i < 30)) 
				{
					data.at(position) = 15;
					data.at(position + 1) = 15;
					data.at(position + 2) = 15;
				}
			}
		}

		setTexture(roofTextures[3], data, width, height);

		std::vector<GLubyte> data2(width * height * 3);
		std::fill(data2.begin(), data2.end(), 25);

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				position = (i * width + j) * 3;
				if ((i < 5 || i >= 35 || j < 5 || j >= 35) ||
					(i >= 10 && i < 15 && j >= 10 && j < 30) ||
					(i >= 25 && i < 30 && j >= 10 && j < 30))
				{
					data2.at(position) = 16;
					data2.at(position + 1) = 16;
					data2.at(position + 2) = 16;
				}
			}
		}

		setTexture(roofTextures[4], data2, width, height);

		std::vector<GLubyte> data3(width * height * 3);
		std::fill(data3.begin(), data3.end(), 30);

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				position = (i * width + j) * 3;
				if ((i < 5 || i >= 35 || j < 5 || j >= 35))
				{
					data3.at(position) = 15;
					data3.at(position + 1) = 15;
					data3.at(position + 2) = 15;
				}
			}
		}

		setTexture(roofTextures[5], data3, width, height);
	}

	void createRoad() 
	{
		std::vector<GLubyte> data;
		for (int i = 0; i < ROAD_WIDTH; i++) {
			if ((i > (ROAD_WIDTH / 2) - 1 && i < (ROAD_WIDTH / 2) + 1) || (i > 2 && i < 5) || (i > ROAD_WIDTH - 5 && i < ROAD_WIDTH - 2)) {
				data.push_back(200);
				data.push_back(200);
				data.push_back(200);
			}
			else {
				data.push_back(0);
				data.push_back(0);
				data.push_back(0);
			}
		}
		setTexture(roadTextures[0], data, ROAD_WIDTH, 1);
	}

	void createRandomWindow(int c) 
	{
		unsigned int width = NUMBER_OF_WINDOWS_WIDTH;
		unsigned int height = NUMBER_OF_WINDOWS_HEIGHT;
		unsigned int position;
		std::vector<GLubyte> data(width * height * WINDOW_AREA * 3);
		int randomX = rand() % 12 + 7;
		std::fill(data.begin(), data.end(), 0);

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				int random = rand() % 150;
				if (random < 12) {
					for (int k = FRAME; k < WINDOW_HEIGHT - FRAME; k++) {
						for (int l = FRAME; l < WINDOW_WIDTH - FRAME; l++) {
							position = (((i * WINDOW_WIDTH) + l + ((j * width * WINDOW_AREA) + (k * WINDOW_WIDTH * width))) * 3);
							if (random < 1) {
								data.at(position) = 255;
								data.at(position + 1) = 255;
								data.at(position + 2) = 255;
							} 
							else if (random < 6) {
								data.at(position) = 150;
								data.at(position + 1) = 150;
								data.at(position + 2) = 150;
							}
							else {
								data.at(position) = 90;
								data.at(position + 1) = 90;
								data.at(position + 2) = 90;
							}
						}
					}
				}
				else {
					int random2 = rand() % 10 + 20;
					for (int k = FRAME; k < WINDOW_HEIGHT - FRAME; k++) {
						for (int l = FRAME; l < WINDOW_WIDTH - FRAME; l++) {
							position = (((i * WINDOW_WIDTH) + l + ((j * width * WINDOW_AREA) + (k * WINDOW_WIDTH * width))) * 3);
							data.at(position) = random2;
							data.at(position + 1) = random2;
							data.at(position + 2) = random2;
						}
					}
				}
			}
		}

		setTexture(textures[c], data, width * WINDOW_WIDTH, height * WINDOW_HEIGHT);
	}

	// nastavi texturu, vytvori mipmapy
	void setTexture(unsigned int textureID, std::vector<GLubyte> data, unsigned int width, unsigned int height) 
	{
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
};

#endif