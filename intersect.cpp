#define _USE_MATH_DEFINES 

#include "intersect.h"
#include "lina.h"
#include "vector.h"
#include "triangle.h"
#include "math.h"


// Given line pq and ccw triangle abc, return whether line pierces triangle. If
// so, also return the barycentric coordinates (u,v,w) of the intersection point
int IntersectLineTriangle(Vector p, Vector q, Vector a, Vector b, Vector c,
                          float &u, float &v, float &w, triangle *tri)
{
    Vector pq = q - p;
    Vector pa = a - p;
    Vector pb = b - p;
    Vector pc = c - p;
    // Test if pq is inside the edges bc, ca and ab. Done by testing
    // that the signed tetrahedral volumes, computed using scalar triple
    // products, are all positive
	Vector m(3);

	// optimierbar
	crossProduct(pq, pc, &m);
	u = scalarProd(&pb, &m); // ScalarTriple(pq, pc, pb);
	v = -scalarProd(&pa, &m); // ScalarTriple(pq, pa, pc);
	if (!SameSign(u, v)) return 0;
	w = ScalarTriple(pq, pb, pa);
	if (!SameSign(u, w)) return 0;

	// optimierte version mit vorberechnungen, scheint nur bedingt weiterzuhelfen
	/*crossProduct(pq, p, &m);
	u = scalarProd(pq, tri->xcb) + scalarProd(m, tri->cb);
	v = scalarProd(pq, tri->xac) + scalarProd(m, tri->ac);
	if (!SameSign(u, v)) return 0;
	w = scalarProd(pq, tri->xba) + scalarProd(m, tri->ba);
	if (!SameSign(u, w)) return 0;*/


    // Compute the barycentric coordinates (u, v, w) determining the
    // intersection point r, r = u*a + v*b + w*c
    float denom = 1.0f / (u + v + w);
    u *= denom;
    v *= denom;
    w *= denom; //  w = 1.0f - u - v;
    return 1;
}



// p: ebene
// u: position
// v: richtung
// lambda: länge
int checkIntersectRayPlane(triangle *p, Vector *u, Vector *dir, float* lambda)
{
	Vector v(3,0);
	v[0] = (*dir)[0];
	v[1] = (*dir)[1];
	v[2] = (*dir)[2];
	normalizeVector(&v);
	
	float scalarprod = scalarProd(&p->normal, &v);
	Vector delta(3,0);
	if(scalarprod == 0)
	{
		return 0;
	}

	delta[0] = p->a[0] - (*u)[0];
	delta[1] = p->a[1] - (*u)[1];
	delta[2] = p->a[2] - (*u)[2];
	*lambda = scalarProd(&p->normal, &delta) / scalarprod;

	if(*lambda <= 0)
	{
		return 0;
	}	

	return 1;

}


bool checkPointInTriangle(Vector& point, triangle* tr) {
	double totalAngles = 0.0f;
	Vector v1 = point - tr->a;
	Vector v2 = point - tr->b;
	Vector v3 = point - tr->c;
	normalizeVector(&v1);
	normalizeVector(&v2);
	normalizeVector(&v3);
	totalAngles += acos(scalarProd(&v1,&v2));
	totalAngles += acos(scalarProd(&v2,&v3));
	totalAngles += acos(scalarProd(&v3,&v1));
	if (fabs(totalAngles - 2*M_PI) <= 0.005)
		return true;
	return false;
}
