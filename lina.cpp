#include "math.h"
#include "lina.h"
#include "float.h"
#include <iostream>

Vector operator+ (Vector u, Vector v)
{
	Vector res(3,0);

	for(int i = 0; i < 3; i++)
	{
		res[i] = u[i] + v[i];
	}
		
	return res;
}

Vector operator* (Vector u, float s)
{
	Vector res(3,0);

	for(int i = 0; i < 3; i++)
	{
		res[i] = u[i] *s ;
	}
		
	return res;
}



Vector operator+= (Vector u, Vector v)
{
	Vector res(3);

	for(int i = 0; i < 3; i++)
	{
		res[i] = u[i] + v[i];
	}
		
	return res;
}


Vector operator- (Vector u, Vector v)
{
	Vector res(3);

	for(int i = 0; i < 3; i++)
	{
		res[i] = u[i] - v[i];
	}
		
	return res;
}

//ostream operator<< (ostream &out, Vector v)
//{
//	out << v[0] << " " << v[1] << " " << v[2];
//	return out;
//}


float scalarProd(Vector v, Vector u)
{
	float res = 0.0;
	for(unsigned int i = 0; i < v.size(); i++)
	{
		res += v[i]*u[i];
	}	
	return res;
}



void normalizeVector(Vector *vec)
{
	float len = sqrtf(pow((double)(*vec)[0],2) + pow((*vec)[1],2) + pow((*vec)[2],2));
	(*vec)[0] /= len;
	(*vec)[1] /= len;
	(*vec)[2] /= len;
}

void crossProduct(Vector u, Vector v, Vector *out)
{
	(*out)[0] = u[1]*v[2] - v[1]*u[2];
	(*out)[1] = v[0]*u[2] - u[0]*v[2];
	(*out)[2] = u[0]*v[1] - u[1]*v[0]; 
}

float ScalarTriple(Vector a, Vector b, Vector c)
{
	Vector m(3);
	crossProduct(a,b,&m);
	return scalarProd(m, c);
}

float getVectorLen(Vector vec)
{
	float len = sqrtf(pow(vec[0],2) + pow(vec[1],2) + pow(vec[2],2));
	return len;
}


float getDistance(Vector u, Vector v)
{
	float res= 0;
	Vector temp(3,0);	

	temp[0] = u[0] - v[0];
	temp[1] = u[1] - v[1];
	temp[2] = u[2] - v[2];

	return getVectorLen(temp);
}

void getOutVector(Vector v, Vector n, Vector *out)
{
	// o + (-i) = 2*n
	// o = 2*n + i
	float dotprod = scalarProd(v,n);
	//dotprod /= dotprod;
	int sign = -1*dotprod / abs(dotprod);
	(*out)[0] = 2*sign*n[0] + v[0];
	(*out)[1] = 2*sign*n[1] + v[1];
	(*out)[2] = 2*sign*n[2] + v[2];

	normalizeVector(out);


}

float SqDistPointSegment(Vector a, Vector b, Vector c)
{
	Vector ab = b-a, ac = c-a, bc = c-b;
	float e = scalarProd(ac, ab);
	// fälle, wo c ausserhalb ab projeziert
	if(e <=0.0f) return scalarProd(ac, ac);
	float f = scalarProd(ab,ab);
	if(e <= f) return scalarProd(bc,bc);
	// fälle, wo c auf ab projeziert
	return scalarProd(ac,ac) - e * e/f;
}

// Clamp n to lie within the range [min, max]
float Clamp(float n, float min, float max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}


// Computes closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
// S2(t)=P2+t*(Q2-P2), returning s and t. Function result is squared
// distance between between S1(s) and S2(t)
float ClosestPtSegmentSegment(Vector p1, Vector q1, Vector p2, Vector q2,
                              float &s, float &t, Vector &c1, Vector &c2)
{
    Vector d1 = q1 - p1; // Direction vector of segment S1
    Vector d2 = q2 - p2; // Direction vector of segment S2
    Vector r = p1 - p2;
	float a = scalarProd(d1, d1); // Squared length of segment S1, always nonnegative
    float e = scalarProd(d2, d2); // Squared length of segment S2, always nonnegative
    float f = scalarProd(d2, r);

    // Check if either or both segments degenerate into points
    if (a <= FLT_EPSILON  && e <= FLT_EPSILON ) {
        // Both segments degenerate into points
        s = t = 0.0f;
        c1 = p1;
        c2 = p2;
        return scalarProd(c1 - c2, c1 - c2);
    }
    if (a <= FLT_EPSILON) {
        // First segment degenerates into a point
        s = 0.0f;
        t = f / e; // s = 0 => t = (b*s + f) / e = f / e
        t = Clamp(t, 0.0f, 1.0f);
    } else {
        float c = scalarProd(d1, r);
        if (e <= FLT_EPSILON ) {
            // Second segment degenerates into a point
            t = 0.0f;
            s = Clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
        } else {
            // The general nondegenerate case starts here
            float b = scalarProd(d1, d2);
            float denom = a*e-b*b; // Always nonnegative

            // If segments not parallel, compute closest point on L1 to L2, and
            // clamp to segment S1. Else pick arbitrary s (here 0)
            if (denom != 0.0f) {
                s = Clamp((b*f - c*e) / denom, 0.0f, 1.0f);
            } else s = 0.0f;

            // Compute point on L2 closest to S1(s) using
            // t = Dot((P1+D1*s)-P2,D2) / Dot(D2,D2) = (b*s + f) / e
            t = (b*s + f) / e;

            // If t in [0,1] done. Else clamp t, recompute s for the new value
            // of t using s = Dot((P2+D2*t)-P1,D1) / Dot(D1,D1)= (t*b - c) / a
            // and clamp s to [0, 1]
            if (t < 0.0f) {
                t = 0.0f;
                s = Clamp(-c / a, 0.0f, 1.0f);
            } else if (t > 1.0f) {
                t = 1.0f;
                s = Clamp((b - c) / a, 0.0f, 1.0f);
            }
        }
    }

    c1 = p1 + d1 * s;
    c2 = p2 + d2 * t;
    return scalarProd(c1 - c2, c1 - c2);
}



int TestSphereCapsule(bowl fix, bowl move)
{
    // Compute (squared) distance between sphere center and capsule line segment
    float dist2 = SqDistPointSegment(move.oldPos, move.pos, fix.pos);

    // If (squared) distance smaller than (squared) sum of radii, they collide
    float radius = fix.radius + move.radius;
    return dist2 <= radius * radius;
}

int TestCapsuleCapsule(bowl capsule1, bowl capsule2)
{
    // Compute (squared) distance between the inner structures of the capsules
    float s, t;
    Vector c1, c2;
    float dist2 = ClosestPtSegmentSegment(capsule1.oldPos, capsule1.pos,
                                          capsule2.oldPos, capsule2.pos, s, t, c1, c2);

    // If (squared) distance smaller than (squared) sum of radii, they collide
    float radius = capsule1.radius + capsule2.radius;
    return dist2 <= radius * radius;
}



bool SameSign(float a, float b)
{
	return a*b >= 0;
}

