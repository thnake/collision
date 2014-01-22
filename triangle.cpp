#include "triangle.h"
#include "lina.h"

triangle::triangle(void)
{
	a.resize(3);
	b.resize(3);
	c.resize(3);
	xac.resize(3);
	xba.resize(3);
	xcb.resize(3);

	cb.resize(3);
	ac.resize(3);
	ba.resize(3);

	normal.resize(3);
}

void triangle::setNormal()
{
	ac = this->a - this->c;
	ba = this->b - this->a;
	cb = this->c - this->b;

	crossProduct(ba, ac, &this->normal);
	normalizeVector(&this->normal);

	crossProduct(this->c, this->b, &this->xcb);
	crossProduct(this->a, this->c, &this->xac);
	crossProduct(this->b, this->a, &this->xba);

}

void triangle::translate(Vector t)
{
	a = a + t;
	b = b + t;
	c = c + t;
}


triangle::~triangle(void)
{

}
