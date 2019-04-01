#ifndef HOUSEFLATBASE_H
#define HOUSEFLATBASE_H

#include "BuildingBase.h"

class HouseFlatBase : public BuildingBase {

public:

	HouseFlatBase();

	void addPosition(float x, float z);
	void processVAO();
	void draw(Shader shader);
	int getPositionsSize();

protected:

	unsigned int _buffer;

	vector<glm::vec3> _positions;
};

#endif