#include "Generator.h"

// kontruktor ulozi startovni pozici generovani
Generator::Generator(float x) 
{
	_x = - MAP_SIZE / 2;
	_z = MAP_SIZE / 2;
}

// funkce prochazi celou mapu a uklada vygenerovane cesty do 
// promenne _roadsArray a zaroven vola funkce pro generovani bloku
void Generator::generate()
{
	bool end_x = false, end_z = false;
	int random_depth, random_width;
	float x = 0, z = 0;

	_roadsArray.push_back(Road(ROAD_WIDTH, MAP_SIZE, ROAD_VERTICAL));
	_roadsArray.at(0).addPosition(_x, _z);

	x += ROAD_WIDTH;

	_roadsArray.push_back(Road(MAP_SIZE - ROAD_WIDTH, ROAD_WIDTH, ROAD_HORIZONTAL));

	while (!end_z) 
	{
		_roadsArray.at(1).addPosition(_x + ROAD_WIDTH, _z - z);

		z += ROAD_WIDTH;

		random_depth = rand() % 600 + 500;

		if ((MAP_SIZE - z) - random_depth < 230) 
		{
			random_depth = MAP_SIZE - z;
			end_z = true;
		}

		random_width = rand() % 600 + 500;

		_roadsArray.push_back(Road(ROAD_WIDTH, random_depth, ROAD_VERTICAL));

		while (!end_x) 
		{
			processBlock(_x + x, _z - z, random_width, random_depth);

			x += random_width;

			_roadsArray.at(_roadsArray.size() - 1).addPosition(_x + x, _z - z);

			x += ROAD_WIDTH;
			random_width = rand() % 600 + 500;

			if ((MAP_SIZE - x) - random_width < 230) 
			{
				end_x = true;

				processBlock(_x + x, _z - z, MAP_SIZE - x, random_depth);

				x = ROAD_WIDTH;
			}
		}

		z += random_depth;

		end_x = false;
	}
}

// funkce pro generovani bloku, postupne se volaji funkce pro generovani kazde
// vnitrni strany bloku, vygenerovane budovy se ukladaji do promennych
// _housesArray, _flatsArray, _skyscrapersArray
void Generator::processBlock(float x, float z, float width, float depth) 
{
	_x_block = x;
	_z_block = z;
	_width_block = width;
	_depth_block = depth;

	processHorizontalLines(false);
	processHorizontalLines(true);
	processVerticalLines(false);
	processVerticalLines(true);
}

// generovani horizontalnich rad
void Generator::processHorizontalLines(bool counter) 
{
	bool build = true;

	float randomX1, randomX2;
	int random;
	float gap = _width_block;
	float last_depth_block, last_width_block;
	float x = _x_block, z = _z_block;

	while (build) {

		random = rand() % 100 + 1;
		int randomBuilding = rand() % 10;

		int location = getLocation(x, z);

		if (gap < 36) {
			build = false;
			continue;
		}

		else if (gap < 72) {

			build = false;
			if (counter) {
				randomX2 = _housesArray.at((360 + (int)(gap * 10) % 360) + randomBuilding).getDepth();
				_housesArray.at((360 + (int)(gap * 10) % 360) + randomBuilding).addPosition(x, z - _depth_block + randomX2);
				_z2max = _z_block - _depth_block + randomX2;
			}
			else {
				randomX2 = _housesArray.at((360 + (int)(gap * 10) % 360) + randomBuilding).getDepth();
				_housesArray.at((360 + (int)(gap * 10) % 360) + randomBuilding).addPosition(x, z);
				_z2 = _z_block - 1 - randomX2;
			}
		}
		else if (gap < 144 && location == SUBURB) {
			build = false;
			if (counter) {
				randomX2 = _flatsArray.at((720 + (int)(gap * 10) - 720) + randomBuilding).getDepth();
				_flatsArray.at((720 + (int)(gap * 10) - 720) + randomBuilding).addPosition(x, z - _depth_block + randomX2);
				_z2max = _z_block - _depth_block + randomX2;
			}
			else {
				randomX2 = _flatsArray.at((720 + (int)(gap * 10) - 720) + randomBuilding).getDepth();
				_flatsArray.at((720 + (int)(gap * 10) - 720) + randomBuilding).addPosition(x, z);
				_z2 = _z_block - 1 - randomX2;
			}
		}
		else if (gap < 200 && location == CENTER) {
			build = false;
			randomX2 = getRandom(SKYSCRAPER);
			if (counter) {
				_skyscrapersArray.push_back(Skyscraper(x, z - _depth_block + randomX2, gap, randomX2));
				_z2max = _z_block - _depth_block + randomX2;
			}
			else {
				_skyscrapersArray.push_back(Skyscraper(x, z, gap, randomX2));
				_z2 = _z_block - 1 - randomX2;
			}
		}
		else if ((location == CENTER && random < 3) ||
			(location == SUBURB && random < 35) ||
			(location == COUNTRY && random < 60) ||
			(location == WILD && random < 99)) {
			randomX1 = getRandom(HOUSE, gap);
			randomX2 = _housesArray.at((360 + (int)(randomX1 * 10) % 360) + randomBuilding).getDepth();
			if (counter) {
				_housesArray.at((360 + (int)(randomX1 * 10) % 360) + randomBuilding).addPosition(x, z - _depth_block + randomX2);
				_z2max = _z_block - _depth_block + randomX2;
			}
			else {
				_housesArray.at((360 + (int)(randomX1 * 10) % 360) + randomBuilding).addPosition(x, z);
				_z2 = _z_block - 1 - randomX2;
			}
			if (x == _x_block) {
				if (counter) _z1max = z - _depth_block + randomX2;
				else _z1 = _z_block - 1 - randomX2;
			}
			x += randomX1;
		}
		else if ((location == CENTER && random < 80) ||
			(location == SUBURB && random < 96) ||
			(location == COUNTRY && random < 99) ||
			(location == WILD && random < 120)) {
			randomX1 = getRandom(BLOCKOFFLATS, gap);
			randomX2 = _flatsArray.at((720 + (int)(randomX1 * 10) - 720) + randomBuilding).getDepth();
			if (counter) {
				_flatsArray.at((720 + (int)(randomX1 * 10) - 720) + randomBuilding).addPosition(x, z - _depth_block + randomX2);
				_z2max = _z_block - _depth_block + randomX2;
			}
			else {
				_flatsArray.at((720 + (int)(randomX1 * 10) - 720) + randomBuilding).addPosition(x, z);
				_z2 = _z_block - 1 - randomX2;
			}
			if (x == _x_block) {
				if (counter) _z1max = z - _depth_block + randomX2;
				else _z1 = _z_block - 1 - randomX2;
			}
			x += randomX1;
		}
		else {
			randomX1 = getRandom(SKYSCRAPER, gap);
			randomX2 = getRandom(SKYSCRAPER);
			if (x == _x_block) {
				if (counter) _z1max = z - _depth_block + randomX2;
				else _z1 = _z_block - 1 - randomX2;
			}
			if (gap < randomX1) {
				if (counter) {
					_skyscrapersArray.push_back(Skyscraper(x, z - _depth_block + randomX2, gap, randomX2));
					_z2max = _z_block - _depth_block + randomX2;
				}
				else {
					_skyscrapersArray.push_back(Skyscraper(x, z, gap, randomX2));
					_z2 = _z_block - 1 - randomX2;
				}
				build = false;
			}
			else {
				if (counter) {
					_skyscrapersArray.push_back(Skyscraper(x, z - _depth_block + randomX2, randomX1, randomX2));
					_z2max = _z_block - _depth_block + randomX2;
				}
				else {
					_skyscrapersArray.push_back(Skyscraper(x, z, randomX1, randomX2));
					_z2 = _z_block - 1 - randomX2;
				}
				x += randomX1;
			}
		}

		x += 12;
		gap = (_x_block + _width_block) - x;
	}
}

// generovani vertikalnich rad
void Generator::processVerticalLines(bool counter) 
{
	bool build = true;
	float x = _x_block;
	float z;

	int randomX1, randomX2;
	int random;
	int gap;
	if (!counter) {
		if (_z1max > _z1) gap = _z1max - _z1;
		else gap = _z1 - _z1max;
		z = _z1;
	}
	else {
		if (_z2max > _z2) gap = _z2max - _z2;
		else gap = _z2 - _z2max;
		z = _z2;
	}

	z -= 12;
	gap -= 24;

	int last_depth_block, last_width_block;

	while (build) {

		random = rand() % 100 + 1;
		int randomBuilding = rand() % 10;

		int location = getLocation(x, z);

		if (gap < 36) {
			build = false;
			continue;
		}

		if (gap < 72 && gap > 0) {
			build = false;
			randomX1 = _housesArray.at((int)(gap * 10) % 360 + randomBuilding).getWidth();
			if (counter) _housesArray.at((int)(gap * 10) % 360 + randomBuilding).addPosition(x + _width_block - randomX1, z);
			else _housesArray.at((int)(gap * 10) % 360 + randomBuilding).addPosition(x, z);
		}
		else if (gap < 144 && (location == SUBURB)) {
			build = false;
			randomX1 = _flatsArray.at((720 + ((int)(gap * 10) - 720)) + randomBuilding).getWidth();
			if (counter) _flatsArray.at((720 + ((int)(gap * 10) - 720)) + randomBuilding).addPosition(x + _width_block - randomX1, z);
			else _flatsArray.at((720 + ((int)(gap * 10) - 720)) + randomBuilding).addPosition(x, z);
		}
		else if (gap < 250 && (location == CENTER)) {
			build = false;
			randomX1 = getRandom(SKYSCRAPER);
			if (counter) _skyscrapersArray.push_back(Skyscraper(x + _width_block - randomX1, z, randomX1, gap));
			else _skyscrapersArray.push_back(Skyscraper(x, z, randomX1, gap));
		}
		else if ((location == CENTER && random < 3) ||
			(location == SUBURB && random < 35) ||
			(location == COUNTRY && random < 60) ||
			(location == WILD && random < 99)) {
			randomX2 = getRandom(HOUSE, gap);
			randomX1 = _housesArray.at((int)(randomX2 * 10) % 360 + randomBuilding).getWidth();
			if (counter) {
				_housesArray.at((int)(randomX2 * 10) % 360 + randomBuilding).addPosition(x + _width_block - randomX1, z);
			}
			else _housesArray.at((int)(randomX2 * 10) % 360 + randomBuilding).addPosition(x, z);
			z -= randomX2;
			gap -= randomX2;
		}
		else if ((location == CENTER && random < 80) ||
			(location == SUBURB && random < 97) ||
			(location == COUNTRY && random < 99) ||
			(location == WILD && random < 120)) {
			randomX2 = getRandom(BLOCKOFFLATS, gap);
			randomX1 = _flatsArray.at((720 + (int)(randomX2 * 10) - 720) + randomBuilding).getWidth();
			if (counter) _flatsArray.at((720 + (int)(randomX2 * 10) - 720) + randomBuilding).addPosition(x + _width_block - randomX1, z);
			else _flatsArray.at((720 + (int)(randomX2 * 10) - 720) + randomBuilding).addPosition(x, z);
			z -= randomX2;
			gap -= randomX2;
		}
		else {
			randomX1 = getRandom(SKYSCRAPER);
			randomX2 = getRandom(SKYSCRAPER, gap);
			if (counter) _skyscrapersArray.push_back(Skyscraper(x + _width_block - randomX1, z, randomX1, randomX2));
			else _skyscrapersArray.push_back(Skyscraper(x, z, randomX1, randomX2));
			z -= randomX2;
			gap -= randomX2;
		}

		z -= 12;
		gap -= 12;
	}
}

// funkce, ktera vrati nahodnou sirku budovy podle typu s ohledem na posledni
// budovu v rade
int Generator::getRandom(int type, int gap) 
{
	float random = 0;
	switch (type)
	{
	case HOUSE:
		random = rand() % 36 + 36;
		break;
	case BLOCKOFFLATS:
		random = rand() % 72 + 72;
		break;
	case SKYSCRAPER:
		random = rand() % 200 + 150;
		break;
	default:
		break;
	}

	if (random > gap) return gap;
	return random;
}

// funkce, ktera vrati nahodnou sirku budovy podle typu
int Generator::getRandom(int type) 
{
	int random = 0;
	switch (type)
	{
	case HOUSE:
		random = rand() % 36 + 36;
		break;
	case BLOCKOFFLATS:
		random = rand() % 72 + 72;
		break;
	case SKYSCRAPER:
		random = rand() % 200 + 150;
		break;
	default:
		break;
	}

	return random;
}

// vrati cast mesta, ve ktere se budova nachazi
int Generator::getLocation(float x, float z)
{
	if (x < CENTER && x > -CENTER && z < CENTER && z > -CENTER) return CENTER;
	if (x < SUBURB && x > -SUBURB && z < SUBURB && z > -SUBURB) return SUBURB;
	if (x < COUNTRY && x > -COUNTRY && z < COUNTRY && z > -COUNTRY) return COUNTRY;
	else return WILD;
}