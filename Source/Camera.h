#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "CatmullRom.h"

// moznosti pohybu kamery
// neni potreba bez pohybu klaveskou a mysi
//enum Camera_Movement {
//	FORWARD,
//	BACKWARD,
//	LEFT,
//	RIGHT
//};

// neni potreba bez pohybu klaveskou a mysi
// defaultni hodnoty kamery
//const float YAW = 0.0f;
//const float PITCH = 0.0f;
//const float SPEED = 2500.0f;
//const float SENSITIVTY = 0.1f;
//const float ZOOM = 45.0f;

class Camera
{
public:

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// neni potreba bez pohybu klaveskou a mysi
	/*float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;*/

	// pomocne promenne pro vypocet pozice a pohledu kamery
	std::vector<CatmullRom> Points;
	int pointCounter;
	glm::vec3 position;
	glm::vec3 focusPosition;
	glm::vec3 lookahead;
	float lookaheadDelta = 0.01f;
	float delta;
	float speed = 300;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : Front(glm::vec3(0.0f, 0.0f, -1.0f))
	{
		Position = position;
		WorldUp = up;
	}

	// vrati view matici
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	// vrati pozici kamery
	glm::vec3 GetPosition() {
		return Position;
	}

	// nastavi bod trajektorie, podle hudby
	void setPointCounter(int PointCounter)
	{
		if (pointCounter != PointCounter)
		{
			pointCounter = PointCounter;
			speed = Points.at(pointCounter).speed;
			delta = lookaheadDelta;
			lookahead = CatmullRomPosition(delta, Points.at(pointCounter).p0, Points.at(pointCounter).p1, Points.at(pointCounter).p2, Points.at(pointCounter).p3);
			position = Points.at(pointCounter).p1;
			Position = position;
			UpdateView(Points.at(pointCounter).point1 - Position);
		}
	}

	// zpracuje klavesku
	// neni potreba bez pohybu klaveskou a mysi
	/*void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}*/

	// zpracuje pohyb mysi
	// neni potreba bez pohybu klaveskou a mysi
	/*void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw = glm::mod(Yaw + xoffset, 360.0f);
		Pitch += yoffset;

		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		updateCameraVectors();
	}*/

	// vrati pozici na Catmull-Rom krivce
	glm::vec3 CatmullRomPosition(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
	{
		// koeficienty
		glm::vec3 a = 2.0f * p1;
		glm::vec3 b = p2 - p0;
		glm::vec3 c = 2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3;
		glm::vec3 d = -p0 + 3.0f * p1 - 3.0f * p2 + p3;

		// vzorec
		glm::vec3 pos = 0.5f * (a + (b * t) + (c * t * t) + (d * t * t * t));

		return pos;
	}

	// updatuje pozici kamery podle toho, jakym zpusobem se zrovna pohybuje
	// na konci vola funkci UpdateView, pro upraveni pohledu kamery.
	// Ke spocitani pohybu kamery na Catmull-Rom krivce potrebujeme rychlost
	// pohybu a deltaTime (cas mezi aktualnim a poslednim framem).
	// Rotace probiha pomoci sinusu a cosinusu, taky potrebujeme znat rychlost.
	int update(float dt)
	{
		static float t_rotation = 0;
		static float t_twopoints2 = 0;

		if (Points.at(pointCounter).type == ROTATE) 
		{
			t_rotation += dt;
			Position = glm::vec3(sin(t_rotation*Points.at(pointCounter).point2.y) * Points.at(pointCounter).speed + Points.at(pointCounter).point1.x, 5000, cos(t_rotation*Points.at(pointCounter).point2.y) * Points.at(pointCounter).speed + Points.at(pointCounter).point1.z);
			UpdateView(Points.at(pointCounter).point1 - Position);
			if (t_rotation >= Points.at(pointCounter).point2.x) {
				t_rotation = 0;
				pointCounter++;
				if (pointCounter >= Points.size()) return 0;
				speed = Points.at(pointCounter).speed;
				delta = lookaheadDelta;
				lookahead = CatmullRomPosition(delta, Points.at(pointCounter).p0, Points.at(pointCounter).p1, Points.at(pointCounter).p2, Points.at(pointCounter).p3);
				position = Points.at(pointCounter).p1;
				Position = position;
				UpdateView(Points.at(pointCounter).point1 - Position);
			}
		}
		else if (Points.at(pointCounter).type == TWOPOINTS2)
		{
			t_twopoints2 += (dt * 0.12f);
			if (t_twopoints2 > 1 && pointCounter != 5)
			{
				t_twopoints2 = 0;
				pointCounter++;
				if (pointCounter >= Points.size()) return 0;
				speed = Points.at(pointCounter).speed;
				delta = lookaheadDelta;
				lookahead = CatmullRomPosition(delta, Points.at(pointCounter).p0, Points.at(pointCounter).p1, Points.at(pointCounter).p2, Points.at(pointCounter).p3);
				position = Points.at(pointCounter).p1;
				Position = position;
				UpdateView(Points.at(pointCounter).point1 - Position);
				return Points.at(pointCounter).volumetric;
			}
			else if (t_twopoints2 > 1) t_twopoints2 = 1;

			glm::vec3 point = CatmullRomPosition(t_twopoints2, Points.at(pointCounter).point1, Points.at(pointCounter).point1, Points.at(pointCounter).point2, Points.at(pointCounter).point2);
			UpdateView(point - Position);
		}
		else
		{
			glm::vec3 direction = { 0, 0, 0 };
			float move = dt * speed;
			while (move > 0.0f) {
				if (position == lookahead)
				{
					delta += lookaheadDelta;
					lookahead = CatmullRomPosition(delta, Points.at(pointCounter).p0, Points.at(pointCounter).p1, Points.at(pointCounter).p2, Points.at(pointCounter).p3);
				}
				float room = distance(lookahead, position);
				float actual = glm::min(move, room);
				direction = (lookahead - position) / room;
				position += direction * actual;
				if (Points.at(pointCounter).type == TWOPOINTS)
				{
					glm::vec3 focus_direction = Points.at(pointCounter).point2 - Points.at(pointCounter).point1;
					float distanceA = distance(Points.at(pointCounter).p1, Points.at(pointCounter).p2);
					float distanceB = distance(Points.at(pointCounter).p1, position);
					focusPosition = Points.at(pointCounter).point1 + focus_direction * (distanceB / distanceA);
				}
				move -= actual;
				if (actual == room) {
					delta += lookaheadDelta;
					if (delta >= 1) delta = 1;
					lookahead = CatmullRomPosition(delta, Points.at(pointCounter).p0, Points.at(pointCounter).p1, Points.at(pointCounter).p2, Points.at(pointCounter).p3);
				}

				if (move > 0.0f && delta >= 1)
				{
					pointCounter++;
					if (pointCounter >= Points.size()) return 0;
					speed = Points.at(pointCounter).speed;
					move = (speed * dt) - actual;
					delta = lookaheadDelta;
					lookahead = CatmullRomPosition(delta, Points.at(pointCounter).p0, Points.at(pointCounter).p1, Points.at(pointCounter).p2, Points.at(pointCounter).p3);
					position = Points.at(pointCounter).p1;
					Position = position;
					if (Points.at(pointCounter).type == TWOPOINTS) focusPosition = Points.at(pointCounter).point1;
					if (Points.at(pointCounter).type == TWOPOINTS2) {
						t_twopoints2 = 0;
						position = Points.at(pointCounter).p0;
						break;
					}
				}
			}

			Position = position;
			if (Points.at(pointCounter).type == LOOKAHEAD) UpdateView((Position + (direction * 1000.0f)) - Position);
			else if (Points.at(pointCounter).type == TWOPOINTS) UpdateView(focusPosition - Position);
			else UpdateView(Points.at(pointCounter).point1 - Position);
		}

		return Points.at(pointCounter).volumetric;
	}

	void init() {
		// nastaveni kamery pro prulet mestem
		// ne vsechen pohyb je po Catmull-Rom krivce, je k tomu ale vyuzita
		// pro jednoduchost Catmull-Rom trida
		// zacatek
		/*Points.push_back(CatmullRom({ 13829, 497, 14236 }, { 13829, 497, 14236 }, { 4961, 695, 4518 }, { 4961, 695, 4518 }, 900, { 4576, 1800, 3259 }, { 0, 0, 0 }, 1, POINT));
		// pokracovani zacatku
		Points.push_back(CatmullRom({ 7743, 633, 7559 }, { 7743, 633, 7559 }, { 4961, 695, 4518 }, { 4961, 695, 4518 }, 900, { 4476, 1700, 3259 }, { 0, 0, 0 }, 1, POINT));
		// rotace kolem domu
		Points.push_back(CatmullRom({ 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 800, { -9053, 600, -2093 }, { 100, 0.45f, 0 }, 1, ROTATE));
		// prelet s mesicem
		Points.push_back(CatmullRom({ -11333, 569, -3906 }, { -11333, 569, -3906 }, { -11615, 569, 7000 }, { -11615, 569, 7000 }, 900, { 1000000, 50, 0 }, { 0, 0, 0 }, 2, POINT));
		// cesta
		Points.push_back(CatmullRom({ -14370, 48, -2331 }, { -14370, 48, -2331 }, { 10126, 48, -2331 }, { 10126, 48, -2331 }, 3000, { 10127, 48, -2331 }, { 0, 0, 0 }, 2, POINT));
		// pohled nahoru
		Points.push_back(CatmullRom({ -2300, 90, 559 }, { -2300, 90, 559 }, { 0, 0, 0 }, { 0, 0, 0 }, 1500, { -13300, 5000, 559 }, { -2300, 10000, 559 }, 1, TWOPOINTS2));*/
		// rotace kolem stredu
		Points.push_back(CatmullRom({ 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 15000, { 0, 700, 0 }, { 1000, 0.24f, 0 }, 2, ROTATE));
		// prulet mestem 1
		Points.push_back(CatmullRom({ -20000, -1000, -22000 }, { -13593, 694, -15348 }, { 12615, 464, 11996 }, { 17000, -1000, 15000 }, 4000, { 0, 1100, 0 }, { 0, 0, 0 }, 1, POINT));
		// na miste otoceni
		Points.push_back(CatmullRom({ 4200, 1100, -5581 }, { 4200, 1100, -5581 }, { 0, 0, 0 }, { 0, 0, 0 }, 1500, { 4238, 900, -2836 }, { 1966, 750, -5296 }, 1, TWOPOINTS2));
		// prulet mestem 2
		Points.push_back(CatmullRom({ -11890, 407, 12745 }, { -11890, 407, 12745 }, { 1140, 407, 12745 }, { 8568, 407, 5590 }, 1500, { 0, 840, 0 }, { 0, 0, 0 }, 1, POINT));
		Points.push_back(CatmullRom({ -11890, 407, 12745 }, { 1140, 407, 12745 }, { 8568, 407, 5590 }, { 8524, 407, -1999 }, 1500, { 0, 840, 0 }, { 0, 0, 0 }, 1, POINT));
		Points.push_back(CatmullRom({ 1140, 407, 12745 }, { 8568, 407, 5590 }, { 8524, 407, -1999 }, { 8524, 407, -1999 }, 1500, { 0, 840, 0 }, { 0, 0, 0 }, 1, POINT));
		// prulet nad domy
		Points.push_back(CatmullRom({ 2449, 492, 10662 }, { -3790, 492, 7090 }, { -7397, 492, -3665 }, { -4026, 492, -10889 }, 900, { -6500, 492, 2500 }, { -7400, 492, -3900 }, 1, TWOPOINTS));
		// prulet vedle mrakodrapu
		Points.push_back(CatmullRom({ -3442, 361, -5535 }, { -3442, 361, -5535 }, { 4470, 430, -5604 }, { 4470, 430, -5604 }, 900, { 10524, 483, -5657 }, { 5446, 752, -3000 }, 2, TWOPOINTS));
		// konec
		Points.push_back(CatmullRom({ 0, 700, 0 }, { 0, 700, 0 }, { 0, 20000, 0 }, { 0, 20000, 0 }, 1200, { 1, 600, 0 }, { 0, 0, 0 }, 1, POINT));

		position = CatmullRomPosition(0.0f, Points.at(0).p0, Points.at(0).p1, Points.at(0).p2, Points.at(0).p3);
		delta = lookaheadDelta;
		lookahead = CatmullRomPosition(delta, Points.at(0).p0, Points.at(0).p1, Points.at(0).p2, Points.at(0).p3);
		Position = position;
		speed = Points.at(0).speed;
	}

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	// neni potreba bez pohybu klaveskou a mysi
	//void updateCameraVectors()
	//{
	//	// Calculate the new Front vector
	//	glm::vec3 front;
	//	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	//	front.y = sin(glm::radians(Pitch));
	//	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	//	Front = glm::normalize(front);
	//	// Also re-calculate the Right and Up vector
	//	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	//	Up = glm::normalize(glm::cross(Right, Front));
	//}

	void UpdateView(glm::vec3 direction)
	{
		Front = glm::normalize(direction);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif