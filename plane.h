#pragma once

#include "lina.h"
#include <vector>
class plane
{
public:
	Vector p;
	Vector normal;
	Vector a;
	Vector b;
	float la;
	float lb;

	void grid();
	plane(Vector point, Vector va, Vector vb, float lena, float lenb);
	plane();
	~plane(void);

};
