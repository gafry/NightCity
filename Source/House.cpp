#include "House.h"

House::House(float width, float depth)
{
	_width = width;
	_depth = depth;

	_height = rand() % 1580 + 750;
	if (_height > 1700) _height += rand() % 750;
	_height /= 10;

	processMesh();
	processRoof();
}
