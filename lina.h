#include "plane.h"
#include "bowl.h"
#include "vector.h"
#include <iostream>


float scalarProd(Vector *v, Vector *u);
float scalarProd(Vector v, Vector u);

void normalizeVector(Vector *vec);

void crossProduct(Vector u, Vector v, Vector *out);

float getVectorLen(Vector v);

float getDistance(Vector u, Vector v);

void getOutVector(Vector v, Vector n, Vector *out);


//Kollisionserkennung

float SqDistPointSegment(Vector a, Vector b, Vector c);

float Clamp(float n, float min, float max);

float ClosestPtSegmentSegment(Vector p1, Vector q1, Vector p2, Vector q2,
                              float &s, float &t, Vector &c1, Vector &c2);

// Sphere-Swept Volume Intersection
int TestSphereCapsule(bowl fix, bowl move);

int TestCapsuleCapsule(bowl capsule1, bowl capsule2);

float ScalarTriple(Vector a, Vector b, Vector c);

bool SameSign(float a, float b);



// Operatoren
Vector operator+ (Vector u, Vector v);
Vector operator+= (Vector u, Vector v);
Vector operator- (Vector u, Vector v);
//ostream& operator<< (ostream &out, Vector v);

