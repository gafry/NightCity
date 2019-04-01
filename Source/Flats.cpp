#include "Flats.h"

Flats::Flats(float width, float depth)
{
	_width = width;
	_depth = depth;

	_height = rand() % 5250 + 1550;
	_height /= 10;

	processMesh();
	processRoof();
}
