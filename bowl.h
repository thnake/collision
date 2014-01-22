#pragma once
#include "vector.h"

class bowl
{
	


public:
	Vector pos;
	Vector oldPos;
	Vector pace;
	float radius;
	bool move;
	bool fixed;
	float gravity;
	float friction;

	bowl(Vector vPos3, Vector vPace3, bool move);
	bowl(void);
	~bowl(void);

	void updatePosition(float deltaT);

};
