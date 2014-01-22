#include "lina.h"
#include "vector.h"
#include "triangle.h"


int IntersectLineTriangle(Vector p, Vector q, Vector a, Vector b, Vector c,
                          float &u, float &v, float &w, triangle* tri);

int checkIntersectRayPlane(triangle p, Vector u, Vector dir, float* lambda);

bool checkPointInTriangle(Vector& point, triangle* tr);
