#pragma once
#include "vector.h"

class triangle
{

public:
	Vector a;
	Vector b;
	Vector c;
	Vector normal;
	void setNormal();
	void translate(Vector t);
	triangle(void);
	~triangle(void);

	Vector xcb;
	Vector xac;
	Vector xba;

	Vector cb, ac, ba;

};

