#pragma once
#include <vector>
#include "triangle.h"


class body
{
public:
	std::vector<triangle> triangles;
	
	body(std::vector<triangle> vT);
	body();
	~body(void);
};
