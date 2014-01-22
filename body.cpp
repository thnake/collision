#include <vector>
#include "lina.h"
#include "body.h"
#include "triangle.h"

body::body(std::vector<triangle> vT)
{
	triangles = vT;
}

body::body()
{
	triangles.resize(1);
}


body::~body(void)
{
}
