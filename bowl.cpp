#include "bowl.h"
#include "math.h"
#include "lina.h"
#include <GL/glut.h>



bowl::bowl(Vector vPos3, Vector vPace3, bool bmove)
{
		oldPos.resize(3);
		pos.resize(3);
		pos[0] = vPos3[0];
		pos[1] = vPos3[1];
		pos[2] = vPos3[2];

		pace.resize(3);
		pace[0] = vPace3[0];
		pace[1] = vPace3[1];
		pace[2] = vPace3[2];

		move = bmove;
		gravity = 0.001f;
		radius = 0.1f;
		friction = 0.8f;
}



bowl::bowl()
{
		oldPos.resize(3);
		pos.resize(3);
		pos[0] = 0.0f;
		pos[1] = 0.0f;
		pos[2] = 0.0f;

		pace.resize(3);
		pace[0] = 0.0f;
		pace[1] = 0.0f;
		pace[2] = 0.0f;

		radius = 0.1f;
		move = true;
		fixed = false;
		gravity = 0.001f;
		friction = 0.8f;
}



void bowl::updatePosition(float deltaT)
{
	if(!fixed)
	{

		oldPos[0] = pos[0];
		oldPos[1] = pos[1];
		oldPos[2] = pos[2];

		pos[0] += pace[0];
		pos[1] += pace[1];
		pos[2] += pace[2];


	}


}


bowl::~bowl(void)
{

}
