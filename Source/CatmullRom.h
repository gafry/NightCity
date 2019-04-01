#ifndef CATMULLROM_H
#define CATMULLROM_H

#define POINT 420			// pohled kamery zameren na konkretni pozici
#define ROTATE 421			// kamera rotuje dokola se pohledem do stredu kruznice
#define LOOKAHEAD 422		// kamera se diva "pred sebe"
#define TWOPOINTS 423		// pohled kamery se spocita pomoci interpolace mezi dvema body
#define TWOPOINTS2 424		// pozice kamery je nehybna, meni se pouze pohled mezi dvema body

// tato trida puvodne slouzila pouze pro ukaldani
// bodu Catmull-Rom krivky, nakonec se zde ale ukladaji i
// jine zpusoby pohybu kamery, ktere nevyuzivaji Catmull-Rom krivku
class CatmullRom 
{
public:

	CatmullRom(glm::vec3 P0, glm::vec3 P1, glm::vec3 P2, glm::vec3 P3, float Speed, glm::vec3 Yaw, glm::vec3 Pitch, int Volumetric, int Type)
	{
		p0 = P0;
		p1 = P1;
		p2 = P2;
		p3 = P3;
		speed = Speed;
		point1 = Yaw;
		point2 = Pitch;
		type = Type;
		volumetric = Volumetric;
	}

	glm::vec3 p0;
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;

	glm::vec3 point1;
	glm::vec3 point2;

	int type;
	int volumetric;

	float speed;
};

#endif