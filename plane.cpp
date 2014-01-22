#include "plane.h"
#include "lina.h"
extern "C"
{
#include "string.h"
}
#include <GL/glut.h>



// va und vb müssen orthogonal sein
// beginnt das zeichnen bei point
plane::plane(Vector point, Vector va, Vector vb, float lena, float lenb)
{
	//memcpy(&p, point, 3);
	p.resize(3);
	p[0] = point[0];
	p[1] = point[1];
	p[2] = point[2];
	//va.resize(3);
	//vb.resize(3);

	a.push_back(va[0]);
	a.push_back(va[1]);
	a.push_back(va[2]);

	b.push_back(vb[0]);
	b.push_back(vb[1]);
	b.push_back(vb[2]);



	la = lena;
	lb = lenb;

	normal.resize(3);
	crossProduct(a,b, &normal); 

	normalizeVector(&a);
	normalizeVector(&b);
}


plane::plane()
{
	
	p.resize(3);
	p[0] = 0.0;
	p[1] = 0.0;
	p[2] = 10;

	a.resize(3);
	a[0] = 1.0;
	a[1] = 0.0;
	a[2] = 0.0;

	b.resize(3);
	b[0] = 0.0;
	b[1] = 0.0;
	b[2] = 1.0;

	la = 10;

	lb = 10;

	normal.resize(3);
	crossProduct(a,b, &normal); 
}



void plane::grid()
{


	glPushMatrix();
	glTranslatef(p[0], p[1],p[2]);
	for(int j = 0; j < la + 1; j++)
	{
		glColor3f(1.0/(j+1), 1.0/(j+1), 1.0 / (j+1));
		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(lb*b[0], lb*b[1], lb* b[2]);
		glEnd();
		glTranslatef(a[0], a[1], a[2]);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(p[0], p[1],p[2]);
	for(int j = 0; j < lb + 1; j++)
	{
		glColor3f(1.0/(j+1), 1.0/(j+1), 1.0 / (j+1));
		glBegin(GL_LINES);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(la*a[0], la*a[1], la*a[2]);
		glEnd();
		glTranslatef(b[0], b[1], b[2]);

	}
	glPopMatrix();

}

plane::~plane(void)
{
}

