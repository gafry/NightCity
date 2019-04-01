#ifndef GENERATOR_H
#define GENERATOR_H

#define MAP_SIZE 30000

#define HOUSE					419
#define BLOCKOFFLATS			420
#define SKYSCRAPER				421

#define CENTER					1500
#define SUBURB					3000
#define COUNTRY					5000
#define WILD					69

#include "Road.h"
#include "House.h"
#include "Flats.h"
#include "Skyscraper.h"

extern vector<House> _housesArray;
extern vector<Flats> _flatsArray;
extern vector<Skyscraper> _skyscrapersArray;
extern vector<Road> _roadsArray;

class Generator {

public:

	Generator(float x);

	void generate();

private:

	float _x;
	float _z;
	float _x_block;
	float _z_block;
	float _width_block;
	float _depth_block;
	float _z1;
	float _z1max;
	float _z2;
	float _z2max;

	void processBlock(float x, float z, float width, float depth);
	void processHorizontalLines(bool counter);
	void processVerticalLines(bool counter);

	int getRandom(int type, int gap);
	int getRandom(int type);
	int getLocation(float x, float z);
};

#endif