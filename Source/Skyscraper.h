#ifndef SKYSCRAPER_H
#define SKYSCRAPER_H

#include "BuildingBase.h"

class Skyscraper : public BuildingBase {

public:

	Skyscraper(float x, float z, float width, float depth);

	void createSkyscraper();
	void draw(Shader shader);

private:

	float _x;
	float _z;
	float _width_base;
	float _depth_base;
	float _height_base;

	glm::mat4 _model;

	int _random2;
	int _random3;
	int _random4;
	int _random5;
};

#endif