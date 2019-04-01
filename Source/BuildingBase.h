#ifndef BUILDINGBASE_H
#define BUILDINGBASE_H

#include "Mesh.h"

class BuildingBase {

public:

	BuildingBase();

	float getWidth() { return _width; }
	float getDepth() { return _depth; }

protected:

	float _width;
	float _height;
	float _depth;

	int _random;

	glm::vec3 _color;

	vector<Mesh> _meshes;

	void processMesh(float min_height = 0.0f, float x = 0.0f, float z = 0.0f);
	void processRoof(float x = 0.0f, float z = 0.0f);
};

#endif
