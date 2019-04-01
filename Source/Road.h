#ifndef ROAD_H
#define ROAD_H

#include "Mesh.h"

#define ROAD_VERTICAL 0
#define ROAD_HORIZONTAL 1

class Road {

public:

	Road(float width, float depth, short type);

	void addPosition(float x, float z);
	void processVAO();
	void draw();

private:

	float _width;
	float _depth;
	short _type;

	unsigned int _buffer;

	vector<glm::vec3> _positions;
	vector<Mesh> _meshes;

	void processRoad();
};

#endif